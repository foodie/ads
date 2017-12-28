#ifndef _CONN_CLIENT_H__
#define _CONN_CLIENT_H__

#include <elf.h>

#ifndef HAVE_HEAD
#define HAVE_HEAD

//网络交互统一的包头
static const unsigned int HEAD_MAGICNUM = 0xECD5BCBC;
typedef struct _head_t
{
    uint32_t    magic_num;	//魔数
    uint32_t    body_len;	//消息体字节长度
    uint64_t    log_id;		//日志唯一ID, 标明一次检索
} head_t;
#endif

typedef struct iovec iovec_t; //发送或接收的多个buf定义

int conn_init(char *ip, uint32_t port, int *new_fd);

int conn_head_read(int sock, head_t *head, void *buf, 
        size_t buf_size, int timeout);
int conn_head_write(int sock, head_t *head, void *buf, 
        int timeout);

#endif
