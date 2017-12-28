#ifndef _CLIENT_BASE_H
#define _CLIENT_BASE_H

#include <netdb.h>
#include <netinet/tcp.h>
#include "serv.h"

#define CLIENT_CONNECT_NONE     0
#define CLIENT_CONNECT_NONBLOCK 1

/**
 * @brief used for building tcp connection
 *
 * @param addr server ip
 * @param port server port
 * @param falgs whether block
 *
 * @return fd
 */
int client_connect(const char *addr, int port, int flags);

/**
 * @brief set the connection non-blocking
 *
 * @param addr server ip
 * @param port server port
 *
 * @return 
 */

int client_tcp_nonblock_connect(const char *addr, int port);


/**
 * @brief used for adclick talking model
 *
 * @param buf
 * @param buf_size
 * @param timeout recv time limitation
 * @param fd    the sock handle
 *
 * @return  head->body_len
 */
int client_read(int fd, head_t *head, char *buf, int buf_size, int timeout);

/**
 * @brief used for sending buffer in head model, the head refer to head + body
 *
 * @param buf the content buffer without head
 * @param buf_size
 * @param timeout recv time limitation
 *
 * @return  head->body_len
 */
int client_write(int fd, head_t *head, char *buf, int timeout);

/**
 * @brief used for convering from host to ip
 *
 * @param host 
 * @param ipbuf 
 *
 * @return
 */
int client_resolve(char *host, char *ipbuf);

/**
 * @brief used for setting non-blocking model
 *
 * @param fd the handle of connection
 *
 * @return
 */
int client_nonblock(int fd);

/**
 * @brief used for setting nodelay model
 *
 * @param fd the handle of connnection
 *
 * @return
 */
int client_tcp_nodelay(int fd);

/**
 * @brief used for keeping alive state for handle
 *
 * @param fd the handle of connection
 *
 * @return
 */
int client_keepalive(int fd);

/**
 * @brief used for normal reading from server, which is useful in talking without head speciation 
 *
 * @param fd    connection handle
 * @param buf   the raw sending buffer 
 * @param buf_size the size of buf
 *
 * @return
 */
int client_normal_read(int fd, char *buf, int buf_size);

/**
 * @brief used for raw write without head speciation
 *
 * @param fd    the target handle
 * @param buf   the content address
 * @param buf_size
 *
 * @return
 */
int client_normal_write(int fd, char *buf, int buf_size);


#endif
