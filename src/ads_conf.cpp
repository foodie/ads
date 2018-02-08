#include "ads_conf.h"

#include "log.h"

int AdsConf::load(const char* fpath, const char* fname)
{
	int ret = 0;
	struct collection_item *config;

	//整体配置
	ret = conf_init(fpath, fname, &config);
	if ( ret ) {
		FATAL("ads conf init failed");
		return ADS_ERROR;
	}

	ret = loadMysqlConf(config);
	if ( ret ) {
		FATAL("config mysql failed");
		return ADS_ERROR;
	}
		
	// 投放配置 ads
	ret = loadAdsConf(config);
	if ( ret ) {
		FATAL("config ads failed");
		return ADS_ERROR;
	}
		
	// 接口配置 api
	ret = loadApiConf(config);
	if ( ret ) {
		FATAL("config api failed");
		return ADS_ERROR;
	}

	// adview配置 adview
	ret = loadAdviewConf(config);
	if ( ret ) {
		FATAL("config adview failed");
		return ADS_ERROR;
	}
	
	// zookeeper配置 zookeeper
	loadZookeeperConf(config);
		
	//读取完毕
	if ( config ) {
		free_ini_config(config);
		config = NULL;
	}
	
	INFO("[%s/%s.conf] init success", fpath, fname);
	return 0;	
}

int AdsConf::loadMysqlConf(struct collection_item *config)
{
	int ret = 0;
	const char* section = "mysql";

	ret = conf_string(config, section, "host", "127.0.0.1", 
			mysql.host, MAX_HOST_LEN);
	if ( ret ) {
		FATAL("config mysql host failed");
		return ADS_ERROR;
	}

	ret = conf_int(config, section, "port", 0, &mysql.port);
	if ( ret ) {
		FATAL("config mysql port failed");
		return ADS_ERROR;
	}

	ret = conf_string(config, section, "db", "mbox", 
			mysql.db, MAX_MYSQL_DB_LEN);
	if ( ret ) {
		FATAL("config mysql db failed");
		return ADS_ERROR;
	}

	ret = conf_string(config, section, "username", "root", 
			mysql.username, MAX_MYSQL_USERNAME_LEN);
	if (ret) {
		FATAL("config mysql username failed");
		return ADS_ERROR;
	}

	ret = conf_string(config, section, "password", "", 
			mysql.password, MAX_MYSQL_PASSWORD_LEN);
	if (ret) {
		FATAL("config mysql password failed");
		return ADS_ERROR;
	}

	ret = conf_string(config, section, "encoding", "utf8", 
			mysql.encoding, MAX_MYSQL_ENCODING_LEN);
	if (ret) {
		FATAL("config mysql encoding failed");
		return ADS_ERROR;
	}

	return ADS_OK;
}

int AdsConf::loadAdsConf(struct collection_item *config)
{
	int ret = 0;
	const char* section = "ads";

	ret = conf_string(config, "ads", "pid_fpath",
			"ads.pid", ads.pid_fpath, MAX_FPATH_LEN);
	if ( ret ) {
		FATAL("config pid_fpath failed");
		return ADS_ERROR;
	}

	ret = conf_string(config, "ads", "ipdat_fpath",
			"", ads.ipdat_fpath, MAX_FPATH_LEN);
	if ( ret ) {
		FATAL("config ipdat_fpath failed");
		return ADS_ERROR;
	}

	ret = conf_string(config, "ads", "ads_host",
			"/", ads.ads_host, MAX_HOST_LEN);
	if ( ret ) {
		FATAL("config ads_host failed");
		return ADS_ERROR;
	}

	ret = conf_string(config, "ads", "material_host",
			"/", ads.material_host, MAX_HOST_LEN);
	if (ret) {
		FATAL("config material_host failed ");
		return ADS_ERROR;
	}

	ret = conf_string(config, "ads", "advertise_api",
			"", ads.advertise_api, MAX_HOST_LEN);
	if (ret) {
		FATAL("config advertise_api failed ");
		return ADS_ERROR;
	}

	return ADS_OK;
}
	
int AdsConf::loadApiConf(struct collection_item *config)
{
	int ret = 0;
	const char* section = "api";

	ret = conf_string(config, section, "ip_validate",
			"off", api.ip_validate, 4);
	if ( ret ){
		FATAL("config ip_validate failed");
		return ADS_ERROR;
	}
		
	ret = conf_string(config, section, "ip_white",
			"", api.ip_white, 1024);
	if (ret){
		FATAL("config ip_white failed");
		return ADS_ERROR;
	}
		
	ret = conf_string(config, section, "ip_black",
			"", api.ip_black, 1024);
	if (ret){
		FATAL("config ip_black failed");
		return ADS_ERROR;
	}
		
	return ADS_OK;
}

int AdsConf::loadAdviewConf(struct collection_item *config)
{
	int ret = 0;
	const char* section = "adview";

	ret = conf_string(config, section, "adview_ekey", 
			"", adview.ekey, 33);
	if ( ret ){
		FATAL("config adview ekey failed");
		return ADS_ERROR;
	}

	ret = conf_string(config, "adview", "adview_ikey",
			"", adview.ikey, 33);
	if ( ret ){
		FATAL("config adview ikey failed");
		return ADS_ERROR;
	}
	
	ret = conf_string(config, "adview", "adview_logo",
			"", adview.logo, 256);
	if ( ret ){
		FATAL("config adview logo failed");
		return ADS_ERROR;
	}

	ret = conf_string(config, "adview", "adview_icon",
			"", adview.icon, 256);
	if ( ret ) {
		FATAL("config adview icon failed");
		return ADS_ERROR;
	}

	return ADS_OK;
}

int AdsConf::loadZookeeperConf(struct collection_item *config)
{
	conf_string(config, "zookeeper", "zk_switch",
				"off", zookeeper.zk_switch, 4);
	conf_string(config, "zookeeper", "zk_host_port",
				"127.0.0.1:2181", zookeeper.zk_host_port, MAX_HOST_PORT_LEN);
	conf_string(config, "zookeeper", "serv_host_port",
				"", zookeeper.serv_host_port, MAX_HOST_PORT_LEN);
	conf_string(config, "zookeeper", "zk_node",
				"/kvs", zookeeper.zk_node, MAX_NODE_NAME_LEN);
	return ADS_OK;
}