#ifndef _ADS_BASE64_H
#define _ADS_BASE64_H

/**
 * 来源:nginx
 */

#include <string>

#ifndef ngx_memcpy
#define ngx_memcpy memcpy
#endif

#ifndef ngx_memzero
#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)
#endif

#define ngx_base64_encoded_length(len)  (((len + 2) / 3) * 4)
#define ngx_base64_decoded_length(len)  (((len + 3) / 4) * 3)

size_t 	ads_encode_base64(const char *src, size_t size, char *dst);
size_t 	ads_encode_base64url(const char *src, size_t size, char *dst);

size_t	ads_decode_base64(const char *src, size_t size, char *dst);
size_t 	ads_decode_base64url(const char *src, size_t size, char *dst);

#endif
/* vim: set ts=4 sw=4 noet: */
