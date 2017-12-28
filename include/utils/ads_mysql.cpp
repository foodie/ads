#include "ads_mysql.h"

#include "ads_core.h"

static ads_mysql_row_t *ads_mysql_new_row(int bitems)
{
	ads_mysql_row_t *row = new (std::nothrow) ads_mysql_row_t(bitems);
	if (row == NULL) {
		return NULL;
	}
	if (!row->is_created()) {
		delete(row);
		return NULL;
	}
	return row;
}

static void ads_mysql_free_row(ads_mysql_row_t *row)
{
	if (row != NULL) {
		delete(row);
	}
}

/**
 * @brief      连接数据库
 *
 * @param      mysql  The mysql
 *
 * @return     -1:失败 | 0:成功
 */
int ads_mysql_connect(ads_mysql_t *msql, ads_mysql_conf_t *conf)
{
	if (msql == NULL || conf == NULL) {
		return ADS_ERROR;
	}

	MYSQL *mysql = &msql->mysql;

	mysql_init(mysql);
	mysql = mysql_real_connect(mysql, conf->host, conf->user, conf->password, 
						conf->db, conf->port, conf->unix_socket, conf->client_flag);
	if (mysql == NULL) {
		return ADS_ERROR;
	}

	// 设置编码
    char encoding_sql[ADS_MAX_MYSQL_ENCODING_NAME_LEN + 10];
    snprintf(encoding_sql, sizeof(encoding_sql), "set names %s", conf->encoding);
    if (mysql_query(mysql, encoding_sql)) {
    	return ADS_ERROR;
    }

    return ADS_OK;
}

/**
 * @brief      查询数据库
 */
ads_mysql_res_t *ads_mysql_query(ads_mysql_t *msql, const char *sql)
{
	MYSQL 		 *mysql = &msql->mysql;
	MYSQL_RES 	 *res;
	MYSQL_FIELD  *fields;
	unsigned int fields_num;

    // 查询sql
    if (mysql_query(mysql, sql)) {
        return NULL;
    }

    // 获取结果集
    res = mysql_store_result(mysql);
    if (res == NULL) {
        return NULL;
    }

    fields 	 	= mysql_fetch_fields(res);
    fields_num  = mysql_num_fields(res);

	ads_mysql_res_t *dst = (ads_mysql_res_t *) malloc( sizeof(ads_mysql_res_t) );
	if (dst == NULL) {
		goto fail;
	}

	dst->res 		= res;
	dst->fields 	= fields;
	dst->fields_num = fields_num;
	dst->row 		= ads_mysql_new_row( (fields_num + 1)/2 );

	if (dst->row == NULL) {
		goto fail;
	}

    return dst;

fail:
	mysql_free_result(res);
	return NULL;
}

/**
 * @brief      从 mysql 结果集中取得一行
 */
ads_mysql_row_t *ads_mysql_fetch_row(ads_mysql_res_t *res)
{
	MYSQL_FIELD  *fields = res->fields;
	unsigned int fields_num = res->fields_num;

	MYSQL_ROW row = mysql_fetch_row(res->res);
	if (row == NULL) {
		return NULL;
	}

	for (unsigned int i = 0; i < fields_num; i++) {
		ads_map_iset(res->row, fields[i].name, row[i], 1);
    }
    return res->row;
}

/**
 * @brief      通过键名获取对应数据
 */
string ads_mysql_get_string(const ads_mysql_row_t *row, const string &key, const string &def="")
{
	if (row == NULL) {
		return def;
	}
	return ads_map_value(row, key, def);
}
int ads_mysql_get_int(const ads_mysql_row_t *row, const string &key, int def=0)
{
	if (row == NULL) {
		return def;
	}
	string val;
	return ads_map_get(row, key, &val) == ADS_EXIST ? atoi(val.c_str()) : def;
}
char ads_mysql_get_char(const ads_mysql_row_t *row, const string &key, char def=0)
{
	if (row == NULL) {
		return def;
	}
	string val;
	return ads_map_get(row, key, &val) == ADS_EXIST ? val[0] : def;
}
long ads_mysql_get_long(const ads_mysql_row_t *row, const string &key, long def=0)
{
	if (row == NULL) {
		return def;
	}
	string val;
	return ads_map_get(row, key, &val) == ADS_EXIST ? atol(val.c_str()) : def;
}
double ads_mysql_get_double(const ads_mysql_row_t *row, const string &key, double def=0)
{
	if (row == NULL) {
		return def;
	}
	string val;
	return ads_map_get(row, key, &val) == ADS_EXIST ? atof(val.c_str()) : def;
}

/**
 * @brief      销毁结果集
 */
void ads_mysql_res_free(ads_mysql_res_t *res)
{
	if (res == NULL) {
		return;
	}
	mysql_free_result(res->res);
	ads_mysql_free_row(res->row);
	free(res);
}

const char *ads_mysql_error(ads_mysql_t *mysql) {
	return mysql_error(&mysql->mysql);
}

void ads_mysql_close(ads_mysql_t *mysql) {
	mysql_close(&mysql->mysql);
}
