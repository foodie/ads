#ifndef _ADS_CONF_H
#define _ADS_CONF_H

#include "ads_def.h"

static const char CONF_FPATH[] = "./conf";
static const char CONF_FNAME[] = "ads";

static const unsigned int MAX_HOST_LEN = 256;
static const unsigned int MAX_HOST_PORT_LEN = 256;

static const unsigned int MAX_MYSQL_DB_LEN = 64;
static const unsigned int MAX_MYSQL_USERNAME_LEN = 64;
static const unsigned int MAX_MYSQL_PASSWORD_LEN = 64;

static const unsigned int MAX_NODE_NAME_LEN = 32;

static const unsigned int MAX_FNAME_LEN = 256;
static const unsigned int MAX_FPATH_LEN = 256;

/* mysql */
typedef struct _mysql_conf_t
{
	char host[MAX_HOST_LEN];
	int  port;
	char db[MAX_MYSQL_DB_LEN];
	char username[MAX_MYSQL_USERNAME_LEN];
	char password[MAX_MYSQL_PASSWORD_LEN];
	char encoding[16];
} mysql_conf_t;

/* ads */
typedef struct _ads_conf_t
{
	char pid_fpath[MAX_FPATH_LEN];
	char hywry_fpath[MAX_FPATH_LEN];
	char ads_host[MAX_HOST_LEN];
	char material_host[MAX_HOST_LEN];
} ads_conf_t;

/* api */
typedef struct _api_conf_t
{
	char ip_validate[4];
	char ip_white[1024];
	char ip_black[1024];
} api_conf_t;

/* adview */
typedef struct _adview_conf_t
{
	char ekey[33];
	char ikey[33];
	char logo[256];
	char icon[256];
} adview_conf_t;

/* zookeeper */
typedef struct _zookeeper_conf_t
{
	char zk_switch[4];
	char zk_host_port[MAX_HOST_PORT_LEN];
	char serv_host_port[MAX_HOST_PORT_LEN];
	char zk_node[MAX_NODE_NAME_LEN];
} zookeeper_conf_t;

typedef struct _redis_conf_t
{
	char host[MAX_HOST_LEN];
	int  port;
} redis_conf_t;

class AdsConf
{
public:
	mysql_conf_t 	 mysql;
	ads_conf_t 		 ads;
	api_conf_t 		 api;
	adview_conf_t 	 adview;
	zookeeper_conf_t zookeeper;
	
public:
	AdsConf() {}
	~AdsConf() {}
	
	int load(const char* fpath, const char* fname);

private:

	int loadMysqlConf(struct collection_item *config);

	int loadAdsConf(struct collection_item *config);
	
	int loadApiConf(struct collection_item *config);

	int loadAdviewConf(struct collection_item *config);

	int loadZookeeperConf(struct collection_item *config);
};

typedef AdsConf g_conf_t;

extern g_conf_t *g_conf;


#endif
/* vim: set ts=4 sw=4 noet: */
