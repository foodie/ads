#ifndef _ADS_CURL_H
#define _ADS_CURL_H

#include <curl/curl.h>
#include "ads_http.h"

// 默认请求方式
#define ADS_CURL_METHOD_DEFAULT 			ADS_HTTP_GET

// 回调函数
typedef size_t (*ads_curl_write_callback_pt) (void*, size_t, size_t, void*);

// curl配置信息
typedef struct _ads_curl_conf_t
{
	int 							method;				// 请求方式, GET | POST
	size_t 							timeout;			// 超时,秒
	size_t 							connect_timeout;	// 超时,秒
	ads_curl_write_callback_pt		write_func;
} ads_curl_conf_t;

// 设置配置信息
#define ads_curl_conf_set_method(conf_ptr, _m) 				((conf_ptr)->method = _m)
#define ads_curl_conf_set_timeout(conf_ptr, _t) 			((conf_ptr)->timeout = _t)
#define ads_curl_conf_set_connect_timeout(conf_ptr, _ct)	((conf_ptr)->connect_timeout = _ct)
#define ads_curl_conf_set_write_callback(conf_ptr, _f) 		((conf_ptr)->write_func = _f)

// curl请求数据
typedef struct _ads_curl_req_t
{
	char 							url[ADS_MAX_HTTP_URL_LEN];		// 请求url
	ads_http_header_t 				*header;						// 请求头
	ads_http_cookie_t 				*cookie;						// 请求cookie
	char							data[ADS_MAX_HTTP_DATA_LEN];	// 请求体
	size_t 							length;							// 请求体长度
} ads_curl_req_t;

// curl响应数据
typedef struct _ads_curl_res_t
{
	int								code;							// 响应码
	ads_http_header_t 				*header;						// 响应头 (暂未解析)
	ads_http_cookie_t 				*cookie;						// 响应cookie (暂未解析)
	char							data[ADS_MAX_HTTP_DATA_LEN];	// 响应体
	size_t 							length;							// 响应体长度
} ads_curl_res_t;

// curl句柄数据
typedef struct _ads_curl_t
{
	CURL 			*curl;	// curl句柄
	ads_curl_conf_t conf;	// 配置信息
	ads_curl_req_t 	req;	// 请求信息
	ads_curl_res_t 	res;	// 响应信息
} ads_curl_t;

// 设置配置数据
#define ads_curl_set_method(conf_ptr, _m) 				((conf_ptr)->conf.method = _m)
#define ads_curl_set_timeout(conf_ptr, _t) 				((conf_ptr)->conf.timeout = _t)
#define ads_curl_set_connect_timeout(conf_ptr, _ct)		((conf_ptr)->conf.connect_timeout = _ct)
#define ads_curl_set_write_callback(conf_ptr, _f) 		((conf_ptr)->conf.write_func = _f)

// 设置请求数据
#define ads_curl_set_url(curl_ptr, _url)				snprintf((curl_ptr)->req.url, ADS_MAX_HTTP_URL_LEN, "%s",  _url)
#define ads_curl_set_data(curl_ptr, _data, _len)		memcpy( (curl_ptr)->req.data, _data, ((curl_ptr)->req.length = _len > ADS_MAX_HTTP_DATA_LEN ? ADS_MAX_HTTP_DATA_LEN : _len) )
#define ads_curl_set_header(curl_ptr, _key, _val)		ads_map_iset((curl_ptr)->req.header, _key, _val, 1)
#define ads_curl_set_cookie(curl_ptr, _key, _val)		ads_map_iset((curl_ptr)->req.cookie, _key, _val, 1)
int 	ads_curl_write_data(ads_curl_t *curl_ptr, const char *data, size_t size);

// 获取响应数据
#define ads_curl_get_code(curl_ptr) 					((curl_ptr)->res.code)
#define ads_curl_get_data(curl_ptr) 					((curl_ptr)->res.data)
#define ads_curl_get_length(curl_ptr) 					((curl_ptr)->res.length)
#define ads_curl_get_header(curl_ptr, key)				ads_map_value((curl_ptr)->res.header, key, "")
#define ads_curl_get_cookie(curl_ptr, key)				ads_map_value((curl_ptr)->res.cookie, key, "")

// 
#define ads_curl_get_config(curl_ptr) 					( &((curl_ptr)->conf) )
#define ads_curl_get_request(curl_ptr) 					( &((curl_ptr)->req) )
#define ads_curl_get_response(curl_ptr) 				( &((curl_ptr)->res) )

//
void		ads_curl_clear_request(ads_curl_t *curl);
void		ads_curl_clear_response(ads_curl_t *curl);

void 		ads_curl_destory_request(ads_curl_t *curl);
void		ads_curl_destory_response(ads_curl_t *curl);

// 
void 		ads_curl_config_default(ads_curl_conf_t *conf);					// 获取默认配置
ads_curl_t 	*ads_curl_init(ads_curl_t **curlp, ads_curl_conf_t *conf=NULL);	// 初始化curl句柄
int 		ads_curl_exec(ads_curl_t *_curl);								// 执行, -1表示不支持的请求类型
void 		ads_curl_close(ads_curl_t *curl);								// 关闭curl句柄,清除数据

/**
 * header不能以\r\n结尾
 */

int ads_curl_get(const char* url, std::string* data, const char* header=NULL);
int ads_curl_post(const char* url, const char* post_data, size_t post_size, std::string* data, const char* header=NULL);

int ads_curl_iget(CURL* curl, const char* url, std::string* data, const char* header=NULL);
int ads_curl_ipost(CURL* curl, const char* url, const char* post_data, size_t post_size, std::string* data, const char* header=NULL);

/**
 * @brief      curl 保存数据回调函数
 *
 * @param      buffer  The buffer 响应buffer
 * @param[in]  size    The size   大小
 * @param[in]  nmemb   The nmemb  长度
 * @param      data    The data   ads_curl_res_t*
 *
 * @return     The data length
 */
size_t ads_curl_write_callback_default(void* buffer, size_t size, size_t nmemb, void* data);


#endif
/* vim: set ts=4 sw=4 noet: */
