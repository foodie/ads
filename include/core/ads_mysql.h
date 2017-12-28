#ifndef _ADS_MYSQL_H
#define _ADS_MYSQL_H

#include <string>
#include <unordered_map>
#include <mysql/mysql.h>

#include "ads_core.h"

using std::string;

static const unsigned int MAX_MYSQL_DB_LEN = 64;
static const unsigned int MAX_MYSQL_USERNAME_LEN = 64;
static const unsigned int MAX_MYSQL_PASSWORD_LEN = 64;
static const unsigned int MAX_MYSQL_ENCODING_LEN = 16;

/* mysql */
typedef struct _ads_mysql_conf_t
{
	char host[MAX_HOST_LEN];
	int  port;
	char db[MAX_MYSQL_DB_LEN];
	char username[MAX_MYSQL_USERNAME_LEN];
	char password[MAX_MYSQL_PASSWORD_LEN];
	char encoding[MAX_MYSQL_ENCODING_LEN];
} ads_mysql_conf_t;

/* The mysql structure: */
typedef struct _ads_mysql_t
{
	MYSQL 	mysql;
} ads_mysql_t;

/* The mysql row structure: */
typedef std::unordered_map<string, string>	ads_mysql_row_t;

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



class AdsMysqlRes;

class AdsMysql
{
public:
	typedef ads_mysql_conf_t AdsMysqlConf;

	enum AdsMysqlConnetion { DisConnected=0, Connected=1 }

	AdsMysql() : connetion(AdsMysqlConnetion::DisConnected), mysql(), res() {}
	~AdsMysql();

	// 连接数据库
	bool connect(const AdsMysqlConf *conf);

	// 断开连接
	void close();

	// 查询数据库
	bool query(const char *sql);
	bool query(const string& sql);

	// 获取查询结果
	AdsMysqlRes& getResult()
	{ return res; }

	// 获取sql错误信息
	string getMysqlError() const
	{ return mysql_error(mysql); }

private:
	AdsMysqlConnetion connetion; // 连接状态
	MYSQL mysql;
	AdsMysqlRes res; // 查询结果
};

typedef std::unordered_map<string, string>	AdsMysqlRow;

class AdsMysqlRes
{
public:
	AdsMysqlRes() : res(NULL), fields(NULL), fieldsNum(0), row() {}
	~AdsMysqlRes();

	// 储存结果集
	bool storeResult(MYSQL *mysql);

	// 销毁结果集
	void freeResult();

	// 获取行结果
	AdsMysqlRow* fetchRow();

	// 
	string& getString(const string& key)
	{ return row[key]; }

	char getChar(const string& key)
	{ return row[key].at(0); }

	int getInt(const string& key)
	{ return atoi(row[key].c_str()); }

	long getLong(const string& key)
	{ return atol(row[key].c_str()); }

private:
	MYSQL_RES 		*res;
	MYSQL_FIELD 	*fields;
	unsigned int 	fieldsNum;

	AdsMysqlRow 	row;
};

#endif
/* vim: set ts=4 sw=4 noet: */
