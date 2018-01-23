#ifndef _ADS_CURL_H
#define _ADS_CURL_H

#include <string>
#include <unordered_map>
#include <curl/curl.h>

#include "ads_http.h"

using std::string;
using std::unordered_map;

/**
 * @brief      curl 保存数据回调函数
 *
 * @param      buffer  The buffer 响应buffer
 * @param[in]  size    The size   大小
 * @param[in]  nmemb   The nmemb  长度
 * @param      data    The data   string*
 *
 * @return     The data length
 */
size_t ads_default_curl_writer(void* buffer, size_t size, size_t nmemb, void* data);

class AdsCurlRequest
{
public:
	void reset();
public:
	string url;
	int method;
	unordered_map<string, string> header;
	unordered_map<string, string> cookie;
	string body;
};

class AdsCurlResponse
{
public:
	void reset();
public:
	int code;
	string body;
};

class AdsCurl
{
public:
	typedef size_t (*AdsCurlWriter) (void*, size_t, size_t, void*);

	AdsCurl() : curl(NULL),
				timeout(3),connecttimeout(3),writer(ads_default_curl_writer),
				request(),response() 
	{
		curl = curl_easy_init();
	}

	~AdsCurl() 
	{
		curl_easy_cleanup(curl);
	}

	// curl
	bool execute();

	void setTimeout(unsigned int t)
	{ timeout = t; }

	void setConnectTimeout(unsigned int t)
	{ connecttimeout = t; }

	void setWriter(AdsCurlWriter w)
	{ writer = w; }

	// request
	void setRequestUrl(const string& u)
	{ request.url = u; }

	void setRequestMethod(int m)
	{ request.method = m; }

	void setRequestHeader(const string& k, const string& v)
	{ request.header[k] = v; }

	void setRequestCookie(const string& k, const string& v)
	{ request.cookie[k] = v; }

	void setRequestBody(const string& b)
	{ request.body = b; }

	// response
	int getResponseCode() const
	{ return response.code; }

	const string& getResponseBody() const
	{ return response.body; }

private:
	CURL *curl;

	unsigned int timeout;
	unsigned int connecttimeout;
	AdsCurlWriter writer;

	AdsCurlRequest request;
	AdsCurlResponse response;
};

#endif
/* vim: set ts=4 sw=4 noet: */
