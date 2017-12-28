#ifndef _ADS_CONF_H
#define _ADS_CONF_H

#include "log.h"
#include "ads_def.h"
#include "ads_mysql.h"

static const char CONF_FPATH[] = "./conf";
static const char CONF_FNAME[] = "ads";
static const unsigned int MAX_IP_LEN = 16;
static const unsigned int MAX_IP_PORT_LEN = 256;
static const unsigned int MAX_NODE_NAME_LEN =32;
static const unsigned int MAX_FNAME_LEN = 256;
static const unsigned int MAX_FPATH_LEN = 256;

typedef struct _zookeeper_conf_t
{
	char zk_switch[4];
	char zk_host_port[MAX_IP_PORT_LEN];
	char serv_host_port[MAX_IP_PORT_LEN];
	char zk_node[MAX_NODE_NAME_LEN];
} zookeeper_conf_t;

typedef struct _redis_conf_t
{
	char host[MAX_IP_LEN];
	unsigned int port;
} redis_conf_t;

class AdsConf
{
public:
	// serv 重载无效
	char serv_fpath[MAX_FPATH_LEN];
	char serv_fname[MAX_FNAME_LEN];
	
	// mysql
	ads_mysql_conf_t mysql;
	
	// ads
	char ads_pid[MAX_FNAME_LEN];
	char ads_host[MAX_IP_PORT_LEN];
	char ads_stuff_host[MAX_IP_PORT_LEN];
	char ads_hywry[256];
	char ads_white_ip[MAX_IP_LEN];
	int  ads_white_ad;
	
	// interior
	char interior_ip_filter_switch[4];
	char interior_ip_white[256];
	char interior_ip_black[256];
	
	// adview
	char adview_ekey[33];
	char adview_ikey[33];
	char adview_logo[256];
	char adview_icon[256];

	// snmi
	int  snmi_key_bits;
	char snmi_key[33];
	
	// zookeeper
	zookeeper_conf_t zookeeper_conf;
	
public:
	AdsConf(){}
	~AdsConf()
	{
		if (config != NULL) {
			free_ini_config(config);
			config = NULL;
		}
	}
	
	/**
	 * @brief 初始化
	 * @return int -1:失败; 0:成功
	**/
	int init()
	{
		config = NULL;
		return 0;
	}
	
	/**
	 * @brief 加载配置
	 * @param [in] fpath 配置文件所在目录
	 * @param [in] fname 配置文件名字
	 * @return int -1:失败; 0:成功
	**/
	int load(const char* fpath, const char* fname)
	{
		int ret = 0;

		//整体配置
		ret = conf_init(fpath, fname, &config);
		if (ret) {
			FATAL("ads conf init failed");
			return -1;
		}
		
		//服务配置 serv
		ret = load_serv();
		if (ret) {
			FATAL("config serv failed");
			return -1;
		}

		// 数据库配置 mysql
		ret = load_mysql();
		if (ret) {
			FATAL("config mysql failed");
			return -1;
		}
		
		// 投放配置 ads
		ret = load_ads();
		if (ret) {
			FATAL("config ads failed");
			return -1;
		}
		
		// 内部接口配置 interior
		ret = load_interior();
		if (ret) {
			FATAL("config interior failed");
			return -1;
		}
		
		// adview配置 adview
		ret = conf_string(config, "adview", "adview_ekey",
				"", adview_ekey, 33);
		if (ret){
			FATAL("config adview_ekey failed");
			return -1;
		}
		ret = conf_string(config, "adview", "adview_ikey",
				"", adview_ikey, 33);
		if (ret){
			FATAL("config adview_ikey failed");
			return -1;
		}
		ret = conf_string(config, "adview", "adview_logo",
				"", adview_logo, 256);
		if (ret){
			FATAL("config adview_logo failed");
			return -1;
		}
		ret = conf_string(config, "adview", "adview_icon",
				"", adview_icon, 256);
		if (ret) {
			FATAL("config adview_icon failed");
			return -1;
		}
		
		// 申米配置 snmi
		ret = conf_int(config, "snmi", "snmi_key_bits",
						256, &snmi_key_bits);
		if (ret){
			FATAL("config snmi_key_bits failed");
			return -1;
		}
		ret = conf_string(config, "snmi", "snmi_key",
				"", snmi_key, 33);
		if (ret){
			FATAL("config snmi_key failed");
			return -1;
		}
			// 右侧填充空格
		for (int i = strlen(snmi_key); i < snmi_key_bits / 8; i++)
			snmi_key[i] = ' ';
		snmi_key[snmi_key_bits / 8] = 0;	
		
		
		// zookeeper配置 zookeeper
		ret = load_zookeeper();
		if (ret) {
			FATAL("config zookeeper failed");
			return -1;
		}
		
		//读取完毕
		if (config){
			free_ini_config(config);
			config = NULL;
		}
		
		INFO("[%s/%s.conf] init success", fpath, fname);
		return 0;	
	}

private:
	struct collection_item *config;

	/**
	 * @brief 加载服务配置
	 * @return int -1:失败; 0:成功
	**/
	int load_serv()
	{
		int ret = 0;
		ret = conf_string(config, "serv", "serv_fpath", 
				"conf/", serv_fpath, MAX_FPATH_LEN);
		if (ret){
			FATAL("config serv_fpath failed ");
			return -1;
		}
		ret = conf_string(config, "serv", "serv_fname", 
				"set", serv_fname, MAX_FNAME_LEN);
		if (ret){
			FATAL("config serv_fname failed");
			return -1;
		}
		return 0;
	}

	/**
	 * @brief 加载mysql配置
	 * @return int -1:失败; 0:成功
	**/
	int load_mysql()
	{
		int ret = 0;
		ret = conf_string(config, "mysql", "mysql_host",
				"127.0.0.1", mysql.host, ADS_MAX_MYSQL_HOST_LEN);
		if (ret) {
			FATAL("config mysql host failed ");
			return -1;
		}
		ret = conf_int(config, "mysql", "mysql_port",
				0, &mysql.port);
		if (ret) {
			FATAL("config mysql port failed ");
			return -1;
		}
		ret = conf_string(config, "mysql", "mysql_db",
				"dsp", mysql.db, ADS_MAX_MYSQL_NODE_NAME_LEN);
		if (ret) {
			FATAL("config mysql db failed ");
			return -1;
		}
		ret = conf_string(config, "mysql", "mysql_user",
				"root", mysql.user, ADS_MAX_MYSQL_NODE_NAME_LEN);
		if (ret) {
			FATAL("config mysql user failed ");
			return -1;
		}
		ret = conf_string(config, "mysql", "mysql_pswd",
				"", mysql.password, ADS_MAX_MYSQL_NODE_NAME_LEN);
		if (ret) {
			FATAL("config mysql password failed ");
			return -1;
		}
		ret = conf_string(config, "mysql", "mysql_encoding",
				"utf8", mysql.encoding, ADS_MAX_MYSQL_ENCODING_NAME_LEN);
		if (ret) {
			FATAL("config mysql encoding failed ");
			return -1;
		}
		
		mysql.unix_socket = 0;
		mysql.client_flag = 0;
		return 0;
	}
	
	/**
	 * @brief 加载投放配置
	 * @return int -1:失败; 0:成功
	**/
	int load_ads()
	{
		int ret = 0;
		// ads_pid
		ret = conf_string(config, "ads", "ads_pid",
				"ads.pid", ads_pid, MAX_FNAME_LEN);
		if (ret) {
			FATAL("config ads_pid failed ");
			return -1;
		}
		// ads_host
		ret = conf_string(config, "ads", "ads_host",
				"http://ads.handyorkad.com", ads_host, MAX_IP_PORT_LEN);
		if (ret) {
			FATAL("config ads_host failed ");
			return -1;
		}
		// ads_stuff_host
		ret = conf_string(config, "ads", "ads_stuff_host",
				"http://mt.handyorkad.com", ads_stuff_host, MAX_IP_PORT_LEN);
		if (ret) {
			FATAL("config ads_stuff_host failed ");
			return -1;
		}
		// ads_hywry
		ret = conf_string(config, "ads", "ads_hywry",
				"", ads_hywry, 256);
		if (ret) {
			FATAL("config ads_hywry failed ");
			return -1;
		}
		// ads_white_ip
		ret = conf_string(config, "ads", "ads_white_ip",
				"", ads_white_ip, MAX_IP_LEN);
		if (ret) {
			FATAL("config ads_white_ip failed ");
			return -1;
		}
		// ads_white_ad
		ret = conf_int(config, "ads", "ads_white_ad",
				0, &ads_white_ad);
		if (ret) {
			FATAL("config ads_white_ad failed ");
			return -1;
		}
		return 0;
	}
	
	/**
	 * @brief 加载内部接口配置
	 * @return int -1:失败; 0:成功
	**/
	int load_interior()
	{
		int ret = 0;
		ret = conf_string(config, "interior", "interior_ip_filter_switch",
				"off", interior_ip_filter_switch, 4);
		if (ret){
			FATAL("config interior_ip_filter_switch failed");
			return -1;
		}
		
		ret = conf_string(config, "interior", "interior_ip_white",
				"off", interior_ip_white, 256);
		if (ret){
			FATAL("config interior_ip_white failed");
			return -1;
		}
		
		ret = conf_string(config, "interior", "interior_ip_black",
				"off", interior_ip_black, 256);
		if (ret){
			FATAL("config interior_ip_black failed");
			return -1;
		}
		
		return 0;
	}
	
	/**
	 * @brief 加载zookeeper配置
	 * @return int -1:失败; 0:成功
	**/
	int load_zookeeper()
	{
		conf_string(config, "zookeeper", "zk_switch",
				"off", zookeeper_conf.zk_switch, 4);
		conf_string(config, "zookeeper", "zk_host_port",
				"127.0.0.1:2181", zookeeper_conf.zk_host_port, MAX_IP_PORT_LEN);
		conf_string(config, "zookeeper", "serv_host_port",
				"", zookeeper_conf.serv_host_port, MAX_IP_PORT_LEN);
		conf_string(config, "zookeeper", "zk_node",
				"/kvs", zookeeper_conf.zk_node, MAX_NODE_NAME_LEN);
		return 0;
	}
};

typedef AdsConf g_conf_t;

extern g_conf_t *g_conf;

/*
struct g_conf_t
{
	char fpath[MAX_FPATH_LEN];
	char fname[MAX_FNAME_LEN];

	//serv
	serv_conf_t serv_conf;
	char serv_fpath[MAX_FPATH_LEN];
	char serv_fname[MAX_FNAME_LEN];

	// mysql
    char mysql_host[MAX_NODE_NAME_LEN];
    char mysql_db[MAX_NODE_NAME_LEN];
    char mysql_user[MAX_NODE_NAME_LEN];
    char mysql_pswd[MAX_NODE_NAME_LEN];
	
	// ads
	char ads_host[256];
	char ads_stuff_host[256];
	char ads_hywry[256];
	char ads_white_ip[16];
	int  ads_white_ad;
	
	// adview
	char adview_ekey[33];
	char adview_ikey[33];
	char adview_logo[256];
	char adview_icon[256];
	
	// snmi
	char snmi_key[33];
	int  snmi_key_bits;
	
	// interior
	char interior_ip_filter_switch[4];
	
	//zookeeper
	char zk_switch[4];
	char zk_host_port[MAX_IP_PORT_LEN];
	char serv_host_port[MAX_IP_PORT_LEN];
	char zk_node[MAX_NODE_NAME_LEN];
};

extern g_conf_t g_conf;

int ads_conf_init();

*/

#endif
/* vim: set ts=4 sw=4 noet: */
