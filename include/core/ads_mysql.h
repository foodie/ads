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
