#include "ads_request.h"

#include <fcgi_stdio.h>
#include "log.h"
#include "utils/ads_string.h"

/**
 * AdsHttpRequest
 */
void AdsHttpRequest::reset()
{
	// general
	method = ADS_HTTP_POST;
	remoteAddress.clear();
	uri.clear();
	query.clear();

	// header
	cookie.clear();
	userAgent.clear();
	referer.clear();

	// body
	body.clear();

    // raw
    uri_raw.clear();
    query_raw.clear();
    cookie_raw.clear();
}

bool AdsHttpRequest::parseFromFcgxRequest(const FCGX_Request *fcgx_request)
{
	if ( fcgx_request == NULL ) {
		WARN("fcgx request is null");
		return false;
	}

	char **envp = fcgx_request->envp;
    char *value;

	// method
    value = FCGX_GetParam("REQUEST_METHOD", envp);
    if (value) {
        if(0 == ads_stricmp("POST", value)) {
            method = ADS_HTTP_POST;
        } else if (0 == ads_stricmp("GET", value)) {
            method = ADS_HTTP_GET;
        }
    }
    
    // remoteAddress
    value = FCGX_GetParam("REMOTE_ADDR", envp);
    if (value){
        remoteAddress = value;
    }

    // uri
    value = FCGX_GetParam("REQUEST_URI", envp);
    if (value){
    	uri_raw = value;

    	// 去掉第一个问号之后的部分
    	if ( uri_raw.find('?') != string::npos ) {
            uri_raw.erase( uri_raw.begin() + uri_raw.find('?'), uri_raw.end() );
        }

        // 切割uri
        size_t p1=0, p2=0;
        string uriParam;
        while( p2 != string::npos ) {
            p2 = uri_raw.find('/', p1);
            uriParam = uri_raw.substr(p1, p2 - p1);
            if ( !uriParam.empty() ) {
            	uri.push_back( uriParam );
            }
            p1 = p2 + 1;
        }
    }

    // query
	value = FCGX_GetParam("QUERY_STRING", envp);
    if (value){
        query_raw = value;

    	size_t p1=0, p2=0, p3=0;
        string queryParam, queryKey, queryValue;
        while( p2 != string::npos ) {
            p2 = query_raw.find('&', p1);
            queryParam = query_raw.substr(p1, p2 - p1);
            if ( !queryParam.empty() ) {
            	p3 = queryParam.find('=');
            	queryKey = queryParam.substr(0, p3);
            	queryValue = p3 != string::npos ? queryParam.substr(p3+1) : "";
            	query[ queryKey ] = queryValue;
            }
            p1 = p2 + 1;
        }
    }

    // cookie
    value = FCGX_GetParam("HTTP_COOKIE", envp);
    if (value){
        cookie_raw = value;

        size_t p1=0, p2=0, p3=0;
        string cookieParam, cookieKey, cookieValue;
        while( p2 != string::npos ) {
            p2 = cookie_raw.find(';', p1);
            cookieParam = cookie_raw.substr(p1, p2 - p1);
            // 去除空格
            cookieParam.erase(0, cookieParam.find_first_not_of(' '));  
			cookieParam.erase(cookieParam.find_last_not_of(' ') + 1);
            if ( !cookieParam.empty() ) {
            	p3 = cookieParam.find('=');
            	cookieKey = cookieParam.substr(0, p3);
            	cookieValue = p3 != string::npos ? cookieParam.substr(p3+1) : "";
            	cookie[ cookieKey ] = cookieValue;
            }
            p1 = p2 + 1;
        }
    }

    // userAgent
    value = FCGX_GetParam("HTTP_USER_AGENT", envp);
    if (value) {
        userAgent = value;
    }

    // referer
    value = FCGX_GetParam("HTTP_REFERER", envp);
    if (value) {
        referer = value;
    }

    // body
    char bodyBuf[ADS_MAX_HTTP_DATA_LEN];
    int length = FCGX_GetStr(bodyBuf, ADS_MAX_HTTP_DATA_LEN, fcgx_request->in);
    body.assign(bodyBuf, length);

    return true;
}

/**
 * AdsHttpResponse
 */
void AdsHttpResponse::reset()
{
	statusCode = ADS_HTTP_OK;
	header.clear();
	cookie.clear();
	body.clear();
}

void AdsHttpResponse::packToFcgxRequest(FCGX_Request *fcgx_request) const
{
	if ( fcgx_request == NULL ) {
		WARN("fcgx request is null");
		return ;
	}

	// header
	for ( auto& h : header ) {
		FCGX_FPrintF(fcgx_request->out, "%s: %s\r\n", h.first.c_str(), h.second.c_str());
	}
    if (cookie.size() > 0) {
        FCGX_PutS("Set-Cookie: ", fcgx_request->out);
        for ( auto& c : cookie) {
            FCGX_FPrintF(fcgx_request->out, "%s=%s;", c.first.c_str(), c.second.c_str());
        }
        FCGX_PutS("\r\n", fcgx_request->out);
    }
    FCGX_PutS("\r\n", fcgx_request->out);

    // 写响应体
    FCGX_PutStr(body.data(), body.size(), fcgx_request->out);
}
