#ifndef _ADS_SHA1_H
#define _ADS_SHA1_H

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

typedef struct {
    uint64_t  bytes;
    uint32_t  a, b, c, d, e, f;
    u_char    buffer[64];
} ngx_sha1_t;


void ngx_sha1_init(ngx_sha1_t *ctx);
void ngx_sha1_update(ngx_sha1_t *ctx, const void *data, size_t size);
void ngx_sha1_final(u_char result[20], ngx_sha1_t *ctx);

void ads_sha1(const void *data, size_t size, char *result);

#endif
/* vim: set ts=4 sw=4 noet: */
