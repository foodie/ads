#ifndef _ADS_MYSQL_H
#define _ADS_MYSQL_H

#include <string>
#include <mysql/mysql.h>

#include "ads_map.h"

using std::string;

/**
 * 对MYSQL数据结构的封装
 */

#define ADS_MAX_MYSQL_HOST_LEN 				 	 16
#define ADS_MAX_MYSQL_NODE_NAME_LEN 			256
#define ADS_MAX_MYSQL_ENCODING_NAME_LEN 		 16

/* The mysql conf structure: */
typedef struct _ads_mysql_conf_t
{
	char host[ADS_MAX_MYSQL_HOST_LEN];
	int  port;
    char db[ADS_MAX_MYSQL_NODE_NAME_LEN];
    char user[ADS_MAX_MYSQL_NODE_NAME_LEN];
    char password[ADS_MAX_MYSQL_NODE_NAME_LEN];
	char *unix_socket; // =0
	long client_flag; // =0
	char encoding[ADS_MAX_MYSQL_ENCODING_NAME_LEN]; // 编码
} ads_mysql_conf_t;

/* The mysql structure: */
typedef struct _ads_mysql_t
{
	MYSQL 		 		mysql;

} ads_mysql_t;

/* The mysql row structure: */
typedef ads_map_t<string, string>	ads_mysql_row_t;

/* The mysql result structure: */
typedef struct _ads_mysql_res_t
{
	MYSQL_RES 		*res;
	MYSQL_FIELD 	*fields;
	unsigned int 	fields_num;
	ads_mysql_row_t *row;
} ads_mysql_res_t;


/**
 * @brief      连接数据库
 */
int ads_mysql_connect(ads_mysql_t *msql, ads_mysql_conf_t *conf);

/**
 * @brief      查询数据库
 */
ads_mysql_res_t *ads_mysql_query(ads_mysql_t *msql, const char *sql);

/**
 * @brief      从 mysql 结果集中取得一行数据
 */
ads_mysql_row_t *ads_mysql_fetch_row(ads_mysql_res_t *res);

/**
 * @brief      通过键名获取对应数据
 */
string 	ads_mysql_get_string(const ads_mysql_row_t *row, const string &key, const string &def="");
int 	ads_mysql_get_int(const ads_mysql_row_t *row, const string &key, int def=0);
char 	ads_mysql_get_char(const ads_mysql_row_t *row, const string &key, char def=0);
long 	ads_mysql_get_long(const ads_mysql_row_t *row, const string &key, long def=0);
double 	ads_mysql_get_double(const ads_mysql_row_t *row, const string &key, double def=0);

/**
 * @brief      销毁结果集
 *
 * @param      res   The resource
 */
void ads_mysql_res_free(ads_mysql_res_t *res);


/**
 * @brief      获取错误信息
 */
const char *ads_mysql_error(ads_mysql_t *mysql);

/**
 * @brief      关闭数据库连接
 */
void ads_mysql_close(ads_mysql_t *mysql);

#endif
/* vim: set ts=4 sw=4 noet: */
