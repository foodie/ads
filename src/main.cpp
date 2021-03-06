#include <sys/time.h>
#include <gflags/gflags.h>
#include <fcgi_stdio.h>
#include <fstream>
#include <unistd.h>
#include <curl/curl.h>
#include <unordered_map>

#include "log.h"
#include "serv.h"
#include "reloader.h"

#include "cluster.h"
#include "ads_def.h"
#include "ads_data.h"
#include "ads_conf.h"
#include "ads_hash.h"
#include "ads_thread.h"
#include "ads_func.h"

#include "ads_http.h"

#include "plugins/controller/ads_controller.h"
#include "plugins/exchange/ads_exchange.h"
#include "plugins/crontab/ads_crontab.h"

#include "core/advertise/ads_advertise_service.h"
#include "core/bidding/ads_bidding_service.h"
#include "core/monitor/ads_monitor_service.h"

#include "utils/ads_ip_location.h"

DEFINE_string(p, CONF_FPATH, "conf path, string");
DEFINE_string(f, CONF_FNAME, "conf file, string");
DECLARE_string(flagfile);

DEFINE_bool(v, false, "print version, bool");
DEFINE_bool(h, false, "help message, bool");

using namespace std;

g_conf_t *g_conf;
reloader_t<g_conf_t> *g_conf_reloader;

g_data_t g_data;

void log_request(AdsHttpRequest *request)
{
    // method
    switch( request->getMethod() ) {
        case ADS_HTTP_UNKNOWN: 
            log_pushnotice("method", "%s", "UNKNOW");
            break;
        case ADS_HTTP_GET:
            log_pushnotice("method", "%s", "GET");
            break;
        case ADS_HTTP_POST:
            log_pushnotice("method", "%s", "GET");
            break;
        default:
            log_pushnotice("method", "%s", "UNKNOW");
            break;
    }

    // remote_addr
    log_pushnotice("remote_addr", "%s", request->getRemoteAddress().c_str());
    // uri
    log_pushnotice("uri", "%s", request->getUriRaw().c_str());
    // query
    log_pushnotice("query", "%s", request->getQueryRaw().c_str());
    // cookie
    log_pushnotice("cookie", "%s", request->getCookieRaw().c_str());
    // ua
    log_pushnotice("user-agent", "%s", request->getUserAgent().c_str());
    // referer
    log_pushnotice("referer", "%s", request->getReferer().c_str());
    // length
    log_pushnotice("length", "%d", request->getBody().size());

}

// 请求回调函数
int callback()
{
	int ret = 0;
	struct timeval start, end;
    gettimeofday(&start, NULL);

    int fd = serv_get_socket();
    if (fd < 0) {
        WARN("module:callback\x02msg:can't get valid socket[%d]", fd);
        return -1;
    }

    uint64_t id = (start.tv_sec * 1000000 + start.tv_usec) * 1000 + (fd < 0 ? 0 : fd);
    log_setlogid(id);

    thread_data_t *p_thd_data = (thread_data_t *) serv_get_user_data();
    if (p_thd_data == NULL) {
        FATAL("get thread data failed");
        return -1;
    }

    if (thread_data_renew(p_thd_data) == ADS_ERROR) {
        FATAL("renew thread data failed");
        return -1;
    }

    FCGX_Request *request = p_thd_data->fcgx_request;

    request->ipcFd = fd;
    request->keepConnection = 1;
    ret = FCGX_Accept_r(request);
    if (ret < 0) {
        FATAL("fcgi read error, %s", strerror(ret));
        return -1;
    }
	
	// 解析请求
	p_thd_data->request->parseFromFcgxRequest(request);
    log_request(p_thd_data->request);
	
    // 调用控制器
    int code;
    string cname = p_thd_data->request->getUri(0);
    AdsController* controller = getController(cname);
	if ( controller == NULL ) {
        code = ADS_HTTP_NOT_FOUND;
    } else {
        code = controller->process(p_thd_data);
    }

    // 处理code
    // 当前只处理404
    //p_thd_data->response->setStatusCode(code);

	// 封装响应
	p_thd_data->response->packToFcgxRequest(request);

    request->keepConnection = 1;
    FCGX_Finish_r(request);

    gettimeofday(&end, NULL);
	float ms = (end.tv_usec-start.tv_usec)/1000 + (end.tv_sec-start.tv_sec)*1000;
	NOTICE("time:%.2lf", ms);
    return 0;
}

// 线程新建时初始化
int init()
{
    thread_data_t *p_thd_data = (thread_data_t *)serv_get_user_data();
    if (p_thd_data == NULL) {
        FATAL("get thread data failed");
        return -1;
    }

	// 初始化线程数据
	if (thread_data_init(p_thd_data) == ADS_ERROR) {
		FATAL("init thread data error!");
		return -1;
	}
	
    try {
        p_thd_data->fcgx_request = new FCGX_Request;
        FCGX_InitRequest(p_thd_data->fcgx_request, 0, 0);
    } catch (const std::exception &e) {
        FATAL("init fcgx request error!");
        return -1;
    }

    return 0;
}

/**
 * 初始化组件
 */
static int plugins_init()
{
    bool ret;
    // exchange
    ret = initExchange();
    if ( !ret ) {
        FATAL("Exchange plugins init failed");
        return -1;
    }

    // crontab
    ret = initCrontab();
    if ( !ret ) {
        FATAL("Crontab plugins init failed");
        return -1;
    }

    // controller
    ret = initController();
    if ( !ret ) {
        FATAL("Controller plugins init failed");
        return -1;
    }

    // iplocation
    ret = initIpLocation();
    if ( !ret ) {
        FATAL("iplocation utils init failed");
        return -1;
    }

    return 0;
}

/**
 * 初始化服务
 */
static int services_init()
{
    bool ret;

    // 广告模块
    ret = AdsAdvertiseService::getInstance().init();
    if ( !ret ) {
        FATAL("AdsAdvertiseService init failed");
        return -1;
    }

    // 竞价模块
    ret = AdsBiddingService::getInstance().init();
    if ( !ret ) {
        FATAL("AdsBiddingService init failed");
        return -1;
    }

    // 监测模块
    ret = AdsMonitorService::getInstance().init();
    if ( !ret ) {
        FATAL("AdsMonitorService init failed");
        return -1;
    }

    return 0;
}

int serv_run(serv_conf_t *serv_conf)
{
    //创建服务
    serv_t *serv = serv_create();
    if (!serv){
        FATAL("create set serv failed");
        return -1;
    }
    serv_load(serv, serv_conf);
    serv_set_raw_callback(serv, callback);
    if (serv_setoptsock(serv, NODELAY) != 0) {
        FATAL("set set nodelay failed");
        return -1;
    }
    if (serv_set_thread_start(serv, init) != 0) {
        FATAL("set set thread start callback failed");
        return -1;
    }
    if (serv_set_user_data(serv, NULL, sizeof(thread_data_t)) != 0){
        FATAL("set set thread data failed");
        return -1;
    }

    if (serv_run(serv) != 0) {
        FATAL("set server run failed");
        return -1;
    }

    if (0 == strcmp("on", g_conf->zookeeper.zk_switch)){
        if (0 != strcmp("", g_conf->zookeeper.serv_host_port)) {
            zhandle_t *zh = register_to_zk(g_conf->zookeeper.zk_host_port,
										   g_conf->zookeeper.zk_node,
										   g_conf->zookeeper.serv_host_port);
            if (NULL == zh) {
                FATAL("register kvs to zookeeper error");
                return -1;
            }
            INFO("register kvs to zookeeper: %s", g_conf->zookeeper.zk_host_port);
        } else {
            FATAL("register kvs to zookeeper, please set local host.");
            return -1;
        }
    }

    serv_join(serv);
    return 0;
}


static void print_version()
{
    fprintf(stdout, "%s", "KEY V3R3B006\n");
    return;
}

static void print_help_usage()
{
    fprintf(stdout, "%s", "Usage: ads [options] ...\n");
    fprintf(stdout, "%s", "Options:\n");
    fprintf(stdout, "%s", "  -h\t\tShow help\n");
    fprintf(stdout, "%s", "  -v\t\tShow version\n");
    return;
}

// 打印进程id,需要先初始化配置信息
static void print_ads_pid()
{
    char pid[16];
    snprintf(pid, 16, "%d", getpid());
    FILE *pid_file = fopen(g_conf->ads.pid_fpath, "w");
    fwrite(pid, 1, strlen(pid), pid_file);
    fclose(pid_file);
}

int main(int argc, char **argv)
{
	int ret;
    signal(SIGPIPE, SIG_IGN);
	srand(time(NULL));
	
    if (log_init("./conf", "log") != 0){
        fprintf(stdout, "log init failed\n");
        return -1;
    }

    FLAGS_flagfile = "./conf/gflags.conf";
    ret = gflags::ParseCommandLineFlags(&argc, &argv, true);
    if (!ret){
        FATAL("use gflags to parse commandline failed");
        return -1;
    }

    if (FLAGS_h){
        print_help_usage();
        return 0;
    }
    if (FLAGS_v){
        print_version();
        return 0;
    }
	
	// 初始化curl
    curl_global_init(CURL_GLOBAL_ALL);
	
    //全局配置
	g_conf_reloader = new (std::nothrow) reloader_t<g_conf_t>(FLAGS_p.c_str(), FLAGS_f.c_str());
	if (g_conf_reloader == NULL) {
		FATAL("conf reloader is null");
		return -1;
	}
	
	//服务配置
	ret = g_conf_reloader->reload();
    if (ret == -1) {
        FATAL("g_conf init failed");
        return -1;
    }
	g_conf = g_conf_reloader->get_content();


    print_ads_pid();

    // 初始化全局变量
    ret = g_data_init();
    if (ret) {
        FATAL("g_data init failed");
        return -1;
    }

    ret = plugins_init();
    if (ret) {
        FATAL("plugins init failed");
        return -1;
    }

    ret = services_init();
    if (ret) {
        FATAL("services init failed");
        return -1;
    }

    //服务配置
	serv_conf_t serv_conf;
    ret = serv_conf_init("./conf", "serv", &serv_conf);
    if (ret) {
        FATAL("set serv conf init failed");
        return -1;
    }

    FCGX_Init();
    ret = serv_run(&serv_conf);
    if (ret){
        FATAL("process suicide");
        raise(SIGKILL);
    }

	log_close();
	curl_global_cleanup();
	
    return 0;
}

/* vim: set ts=4 sw=4 noet: */
