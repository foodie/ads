#include "ads_mysql.h"

/**
 * AdsMysql
 */
AdsMysql::~AdsMysql()
{
	close();
}

bool AdsMysql::connect(const AdsMysqlConf *conf)
{
	close();

	mysql_init(&mysql);
	MYSQL *pMysql = mysql_real_connect(&mysql, conf->host, conf->username, conf->password, 
						conf->db, conf->port, 0, 0);
	if (pMysql == NULL) {
		return false;
	}

	// 设置编码
    string sql("set names ");
    sql.append(conf->encoding);
	query(sql);

    connetion = Connected;
    return true;
}

void AdsMysql::close()
{
	if ( connetion == Connected ) {
		mysql_close(&mysql);
		connetion = DisConnected;
	}
}

bool AdsMysql::query(const char *sql)
{
    // 查询sql
    if ( mysql_query(&mysql, sql) ) {
        return false;
    }

    // 保存结果
    if( !res.storeResult(&mysql) ) {
    	return false;
    }

	return true;
}

bool query(const string& sql)
{ 
	return query(sql.c_str()); 
}


/**
 * AdsMysqlRes
 */
AdsMysqlRes::~AdsMysqlRes()
{
	freeResult();
}

bool AdsMysqlRes::storeResult(MYSQL *mysql)
{
	res = mysql_store_result(mysql);
	if ( res == NULL ) {
		return false;
	}

	fields 	 	= mysql_fetch_fields(res);
    fieldsNum  	= mysql_num_fields(res);

    return true;
}

void AdsMysqlRes::freeResult()
{
	if ( res != NULL ) {
		mysql_free_result(res);
		res = NULL;
	}
}

AdsMysqlRow* AdsMysqlRes::fetchRow()
{
	row.clear();

	MYSQL_ROW m_row = mysql_fetch_row(res);
	if (m_row == NULL) {
		return NULL;
	}

	for (unsigned int i = 0; i < fieldsNum; i++) {
		row[ fields[i].name ] = m_row[i];
    }

    return &row;
}