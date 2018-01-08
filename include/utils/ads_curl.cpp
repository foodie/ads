#include "utils/ads_curl.h"

/**
 * @brief      curl 保存数据回调函数
 */
size_t ads_default_curl_writer(void* buffer, size_t size, size_t nmemb, void* data)
{
	if (data == NULL) {
		return 0;
	}

	size_t length = nmemb * size;
   	((string*) data)->assign( (const char*)buffer, length );

    return length;
}


/**
 * AdsCurlRequest
 */
void AdsCurlRequest::reset()
{
	url.clear();
	method = ADS_HTTP_GET;
	header.clear();
	cookie.clear();
	body.clear();
}

/**
 * AdsCurlResponse
 */
void AdsCurlResponse::reset()
{
	code = 0;
	body.clear();
}

/**
 * AdsCurl
 */
bool AdsCurl::execute()
{
	if (curl == NULL) {
		return false;
	}

	response.reset();

	curl_easy_setopt(curl, CURLOPT_URL, request.url.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response.body);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, connecttimeout);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

	// 添加请求头
	struct curl_slist *header_list = NULL;
	string headerLine;

	for ( auto& itr : request.header ) {
		headerLine = itr.first + ": " + itr.second;
		header_list = curl_slist_append(header_list, headerLine.c_str());
	}

	if (request.cookie.size() > 0) {
		headerLine = "Cookie: ";
		for ( auto& itr : request.cookie) {
			headerLine += itr.first + "=" + itr.second + ";";
		}
		header_list = curl_slist_append(header_list, headerLine.c_str());
	}

	if ( header_list != NULL ) {
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
	}
	
	// 添加请求体
    switch( request.method ) {
    	case ADS_HTTP_GET:
    		curl_easy_setopt(curl, CURLOPT_POST, 0);
    		break;
    	case ADS_HTTP_POST:
    		curl_easy_setopt(curl, CURLOPT_POST, 1);
    		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.body.data());
			curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, request.body.size());
    		break;
    	default: return false;
    }

    // 
    CURLcode code = curl_easy_perform(curl);

    // 释放请求头
   	curl_slist_free_all(header_list);

   	request.reset();

   	if ( code != CURLE_OK ) {
   		return false;
   	}

	return true;
}
