#ifndef _ADS_HTTP_H
#define _ADS_HTTP_H

#include <string>
#include "ads_core.h"
#include "ads_array.h"
#include "ads_map.h"

#define ADS_MAX_HTTP_IP_LEN				  	  16
#define ADS_MAX_HTTP_UA_LEN				 	 256
#define ADS_MAX_HTTP_URL_LEN 				4096
#define ADS_MAX_HTTP_DATA_LEN 				8192

/* 请求方式 */
#define ADS_HTTP_UNKNOWN                   0x0001
#define ADS_HTTP_GET                       0x0002
#define ADS_HTTP_HEAD                      0x0004
#define ADS_HTTP_POST                      0x0008
#define ADS_HTTP_PUT                       0x0010
#define ADS_HTTP_DELETE                    0x0020
#define ADS_HTTP_MKCOL                     0x0040
#define ADS_HTTP_COPY                      0x0080
#define ADS_HTTP_MOVE                      0x0100
#define ADS_HTTP_OPTIONS                   0x0200
#define ADS_HTTP_PROPFIND                  0x0400
#define ADS_HTTP_PROPPATCH                 0x0800
#define ADS_HTTP_LOCK                      0x1000
#define ADS_HTTP_UNLOCK                    0x2000
#define ADS_HTTP_PATCH                     0x4000
#define ADS_HTTP_TRACE                     0x8000

/* 响应码 */
#define ADS_HTTP_CONTINUE                  100
#define ADS_HTTP_SWITCHING_PROTOCOLS       101
#define ADS_HTTP_PROCESSING                102

#define ADS_HTTP_OK                        200
#define ADS_HTTP_CREATED                   201
#define ADS_HTTP_ACCEPTED                  202
#define ADS_HTTP_NO_CONTENT                204
#define ADS_HTTP_PARTIAL_CONTENT           206

#define ADS_HTTP_SPECIAL_RESPONSE          300
#define ADS_HTTP_MOVED_PERMANENTLY         301
#define ADS_HTTP_MOVED_TEMPORARILY         302
#define ADS_HTTP_SEE_OTHER                 303
#define ADS_HTTP_NOT_MODIFIED              304
#define ADS_HTTP_TEMPORARY_REDIRECT        307
#define ADS_HTTP_PERMANENT_REDIRECT        308

#define ADS_HTTP_BAD_REQUEST               400
#define ADS_HTTP_UNAUTHORIZED              401
#define ADS_HTTP_FORBIDDEN                 403
#define ADS_HTTP_NOT_FOUND                 404
#define ADS_HTTP_NOT_ALLOWED               405
#define ADS_HTTP_REQUEST_TIME_OUT          408
#define ADS_HTTP_CONFLICT                  409
#define ADS_HTTP_LENGTH_REQUIRED           411
#define ADS_HTTP_PRECONDITION_FAILED       412
#define ADS_HTTP_REQUEST_ENTITY_TOO_LARGE  413
#define ADS_HTTP_REQUEST_URI_TOO_LARGE     414
#define ADS_HTTP_UNSUPPORTED_MEDIA_TYPE    415
#define ADS_HTTP_RANGE_NOT_SATISFIABLE     416
#define ADS_HTTP_MISDIRECTED_REQUEST       421
#define ADS_HTTP_TOO_MANY_REQUESTS         429

bool ads_is_ip(const char *ip);


long ads_ip2long(const char *ip);
void ads_long2ip(long l_ip, char ip);

#endif
/* vim: set ts=4 sw=4 noet: */
