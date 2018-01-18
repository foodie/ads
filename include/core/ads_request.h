#ifndef _ADS_REQUEST_H
#define _ADS_REQUEST_H

#include <string>
#include <vector>
#include <unordered_map>
#include "ads_http.h"

using std::string;
using std::vector;
using std::unordered_map;

class FCGX_Request;

class AdsHttpRequest {
public:

	AdsHttpRequest() 
		: method(ADS_HTTP_POST), remoteAddress(), 
		  uri_raw(), uri(), query_raw(), query(),
		  cookie_raw(), cookie(), userAgent(), referer(),
		  body()
	{
		reset();
	}

	~AdsHttpRequest() {}

	void reset();

	bool parseFromFcgxRequest(const FCGX_Request *fcgx_request);

public:

	// general
	int getMethod() const 
	{ 
		return method; 
	}

	const string& getRemoteAddress() const 
	{ 
		return remoteAddress; 
	}

	const string& getUriRaw() const
	{
		return uri_raw;
	}
	string getUri(unsigned int n, const string& def="") const 
	{ 
		return uri.size() > n ? uri.at(n) : def; 
	}

	const string& getQueryRaw() const
	{
		return query_raw;
	}
	string getQuery(const string& k, const string& def="") const
	{
		auto itr = query.find(k);
		return itr != query.end() ? itr->second : def;
	}

	// header
	const string& getCookieRaw() const
	{
		return cookie_raw;
	}
	string getCookie(const string& k, const string& def="") const
	{
		auto itr = cookie.find(k);
		return itr != cookie.end() ? itr->second : def;
	}

	const string& getUserAgent() const
	{ 
		return userAgent; 
	}

	const string& getReferer() const
	{ 
		return referer; 
	}

	// body
	const string& getBody() const
	{ 
		return body; 
	}

private:
	// general
	int method;
	string remoteAddress;
	string uri_raw;
	vector<string> uri;
	string query_raw;
	unordered_map<string, string> query;

	// header
	string cookie_raw;
	unordered_map<string, string> cookie;
	string userAgent;
	string referer;

	// body
	string body;
};

class AdsHttpResponse {
public:
	AdsHttpResponse() 
		:statusCode(ADS_HTTP_OK), header(), cookie(), body() 
	{
		reset();
	}
	~AdsHttpResponse() {}

	void reset();
	void packToFcgxRequest(FCGX_Request *fcgx_request) const;

public:
	void setStatusCode(int code)
	{
		statusCode = code;
	}

	void setHeader(const string& k, const string& v)
	{
		header[k] = v;
	}

	void setCookie(const string& k, const string& v)
	{
		cookie[k] = v;	
	}

	string getBody() const
	{
		return body;
	}

private:
	int statusCode;
	unordered_map<string, string> header;
	unordered_map<string, string> cookie;
	string body;
};

#endif
