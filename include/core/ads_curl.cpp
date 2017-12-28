#include "ads_curl.h"


int ads_curl_write_data(ads_curl_t *curl_ptr, const char *data, size_t size)
{
	// 计算可写入长度
	size_t *length = &(curl_ptr->req.length);
	size = (size + *length > ADS_MAX_HTTP_DATA_LEN ? ADS_MAX_HTTP_DATA_LEN - *length : size); 
	memcpy( curl_ptr->req.data + *length, data, size );
	*length += size;
	return size;
}

void ads_curl_clear_request(ads_curl_t *curl)
{
	ads_curl_req_t *req = &curl->req;
	memset(req->url, 0, ADS_MAX_HTTP_URL_LEN);
	req->header->clear();
	req->cookie->clear();
	memset(req->data, 0, ADS_MAX_HTTP_DATA_LEN);
	req->length = 0;
}

void ads_curl_clear_response(ads_curl_t *curl)
{
	ads_curl_res_t *res = &curl->res;
	res->code = 0;
	res->header->clear();
	res->cookie->clear();
	memset(res->data, 0, ADS_MAX_HTTP_DATA_LEN);
	res->length = 0;
}

void ads_curl_destory_request(ads_curl_t *curl)
{
	ads_curl_req_t *req = &curl->req;
	delete req->header;
	delete req->cookie;
}

void ads_curl_destory_response(ads_curl_t *curl)
{
	ads_curl_res_t *res = &curl->res;
	delete res->header;
	delete res->cookie;
}

void ads_curl_config_default(ads_curl_conf_t *conf)
{
	conf->method = ADS_CURL_METHOD_DEFAULT;
	conf->timeout = 3;
	conf->connect_timeout = 3;
	conf->write_func = ads_curl_write_callback_default;
}

ads_curl_t *ads_curl_init(ads_curl_t **curlp, ads_curl_conf_t *conf)
{
	ads_curl_t *curl;
	if (curlp == NULL) {
		curl = (ads_curl_t*) malloc( sizeof(ads_curl_t) );
		if (curl == NULL) {
			return NULL;
		}
		memset(curl, 0, sizeof(ads_curl_t));
		// curl
		curl->curl = curl_easy_init();
		if (curl->curl == NULL) {
			goto CURL_INIT_ERROR;
		}
		// config
		if (conf == NULL) {
			ads_curl_config_default(&curl->conf);
		}
		// req header
		curl->req.header = new (std::nothrow) ads_http_header_t;
		if (curl->req.header == NULL) {
			goto CURL_INIT_ERROR;
		}
		if (ads_map_init(curl->req.header, 6)) {
			goto CURL_INIT_ERROR;
		}
		// req cookie
		curl->req.cookie = new (std::nothrow) ads_http_cookie_t;
		if (curl->req.cookie == NULL) {
			goto CURL_INIT_ERROR;
		}
		if (ads_map_init(curl->req.cookie, 6)) {
			goto CURL_INIT_ERROR;
		}
		// res header
		curl->res.header = new (std::nothrow) ads_http_header_t;
		if (curl->res.header == NULL) {
			goto CURL_INIT_ERROR;
		}
		if (ads_map_init(curl->res.header, 6)) {
			goto CURL_INIT_ERROR;
		}
		// res cookie
		curl->res.cookie = new (std::nothrow) ads_http_cookie_t;
		if (curl->res.cookie == NULL) {
			goto CURL_INIT_ERROR;
		}
		if (ads_map_init(curl->res.cookie, 6)) {
			goto CURL_INIT_ERROR;
		}
	} else {
		curl = *curlp;
	}
	
	// 复制配置
	if (conf != NULL) {
		memcpy( &curl->conf, conf, sizeof(ads_curl_conf_t) );
	}
	return curl;

CURL_INIT_ERROR:
	ads_curl_close(curl);
	return NULL;
}

void ads_curl_close(ads_curl_t *curl)
{
	if (curl == NULL) {
		return;
	}
	curl_easy_cleanup(curl->curl);
	ads_curl_destory_request(curl);
	ads_curl_destory_response(curl);
	free(curl);
}

int ads_curl_exec(ads_curl_t *_curl)
{
	ads_curl_conf_t *_conf = &_curl->conf;
	ads_curl_req_t  *_req  = &_curl->req;
	ads_curl_res_t  *_res  = &_curl->res;
	CURL 			*curl  = _curl->curl;
	
	ads_curl_clear_response(_curl);
	//curl_easy_reset(curl);

	struct curl_slist *header_list = NULL;
	std::string header_line;
	// 添加请求头
	ads_http_header_t::iterator header_itr;
	for (header_itr = _req->header->begin(); header_itr != _req->header->end(); header_itr++) {
		header_line = ads_map_iterator_key(header_itr) + ": " + ads_map_iterator_value(header_itr);
		header_list = curl_slist_append(header_list, header_line.c_str());
	}
	// 添加cookie
	std::string cookie_str = ads_http_cookie_to_string(_req->cookie);
	if (cookie_str.size() > 0) {
		header_line = "Cookie: " + cookie_str;
		header_list = curl_slist_append(header_list, header_line.c_str());
	}

	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, 		header_list);
	curl_easy_setopt(curl, CURLOPT_URL, 			_req->url);
	
    switch(_conf->method) {
    	case ADS_HTTP_GET:
    		curl_easy_setopt(curl, CURLOPT_POST, 			0);
    		break;
    	case ADS_HTTP_POST:
    		curl_easy_setopt(curl, CURLOPT_POST, 			1);
    		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, 	  	_req->data);
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, 	_req->length);
    		break;
    	default:
			return -1;
    }

	curl_easy_setopt(curl, CURLOPT_READFUNCTION, 	NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, 	_conf->write_func);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, 		_res);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 		1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 	_conf->connect_timeout);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 		_conf->timeout);

    _res->code = curl_easy_perform(curl);
   	curl_slist_free_all(header_list);
    ads_curl_clear_request(_curl);

	return _res->code;
}



size_t ads_curl_string_write_callback(void* buffer, size_t size, size_t nmemb, void* data)
{
	if (data == NULL) return 0;
	size_t length = nmemb * size; // 字节长度
    ((std::string*) data)->assign((char*) buffer, length);
    return length;
}

int ads_curl_get(const char* url, std::string* data, const char* header)
{
	CURL* curl = curl_easy_init();
	int ret = ads_curl_iget(curl, url, data, header);
	curl_easy_cleanup(curl);
	return ret;
}

int ads_curl_post(const char* url, const char* post_data, size_t post_size, std::string* data, const char* header)
{
	CURL* curl = curl_easy_init();
    int ret = ads_curl_ipost(curl, url, post_data, post_size, data, header);
	curl_easy_cleanup(curl);
	return ret;
}

int ads_curl_iget(CURL* curl, const char* url, std::string* data, const char* header)
{
	if (curl == NULL) {
		return -1;
	}
	struct curl_slist *header_list = NULL;
	if (header != NULL) {
		header_list = curl_slist_append(header_list, header);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
	}
	curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ads_curl_string_write_callback);	// write function
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);			// body
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(header_list);
	
	return res;
}

int ads_curl_ipost(CURL* curl, const char* url, const char* post_data, size_t post_size, std::string* data, const char* header)
{
	if (curl == NULL) {
		return -1;
	}
	struct curl_slist *header_list = NULL;
	if (header != NULL) {
		header_list = curl_slist_append(header_list, header);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
	}
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, post_size);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);  
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ads_curl_string_write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
    CURLcode res = curl_easy_perform(curl);
    curl_slist_free_all(header_list);
    return res;
}


/**
 * @brief      curl 保存数据回调函数
 */
size_t ads_curl_write_callback_default(void* buffer, size_t size, size_t nmemb, void* data)
{
	if (data == NULL) {
		return 0;
	}
	ads_curl_res_t *res = (ads_curl_res_t*) data;

	// 计算响应数据字节数
	size_t length = nmemb * size;
	// 判断数据是否溢出
	res->length = length > ADS_MAX_HTTP_DATA_LEN ? ADS_MAX_HTTP_DATA_LEN : length;
	// 复制数据
    memcpy(res->data, buffer, res->length);
    return length;
}
