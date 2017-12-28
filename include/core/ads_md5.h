#ifndef _ADS_MD5_H
#define _ADS_MD5_H

/**
 * 来源:nginx
 */

#include <sys/types.h>
#include <stdint.h>
#include <string.h>

#ifndef ngx_memcpy
#define ngx_memcpy memcpy
#endif

#ifndef ngx_memzero
#define ngx_memzero(buf, n)       (void) memset(buf, 0, n)
#endif

typedef struct {
    uint64_t  bytes;
    uint32_t  a, b, c, d;
    u_char    buffer[64];
} ngx_md5_t;


void ngx_md5_init(ngx_md5_t *ctx);
void ngx_md5_update(ngx_md5_t *ctx, const void *data, size_t size);
void ngx_md5_final(u_char result[16], ngx_md5_t *ctx);


void ads_md5(const void *data, size_t size, char *result);

#endif
/* vim: set ts=4 sw=4 noet: */
