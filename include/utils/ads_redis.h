#ifndef __ADS_REDIS_H_
#define __ADS_REDIS_H_

#include <hiredis/hiredis.h>
#include <string>

/**
 * @brief 连接redis数据库
 * @return int
 * @retval -1:连接失败; 0:连接成功
**/
int ads_redis_connect(redisContext **redis_ptr, const char *host, int port);

/**
 * @brief 切换数据库
 * @return int
 * @retval -1:连接失败; 0:连接成功
**/
int ads_redis_select(redisContext *redis_ptr, int index);

// key相关操作 //

/**
 * @brief key存在时删除key
 * @return int
 * @retval -1:执行失败; 0:key不存在; 1:删除成功
**/
int ads_redis_del(redisContext *redis_ptr, const char *key_name);

/**
 * @brief key是否存在
 * @return int
 * @retval -1:执行失败; 0:不存在; 1:存在
**/
int ads_redis_exists(redisContext *redis_ptr, const char *key_name);

/**
 * @brief 修改key的名称,若new_key已经存在,则覆盖旧值
 * @return int
 * @retval -1:执行失败; 0:执行成功
**/
int ads_redis_rename(redisContext *redis_ptr, const char *key_name, const char *new_key_name);

/**
 * @brief 当new_key不存在时修改key的名称
 * @return int
 * @retval -1:执行失败; 0:new_key存在,rename失败; 1:new_key不存在,rename成功
**/
int ads_redis_renamenx(redisContext *redis_ptr, const char *key_name, const char *new_key_name);

/**
 * @brief 设置key的过期时间,以秒计
 * @return int
 * @retval -1:执行失败; 0:key不存在,或设置失败; 1:设置成功
**/
int ads_redis_expire(redisContext *redis_ptr, const char *key_name, long seconds);

/**
 * @brief 设置key过期的时间戳,以秒计
 * @return int
 * @retval -1:执行失败; 0:key不存在,或设置失败; 1:设置成功
**/
int ads_redis_expireat(redisContext *redis_ptr, const char *key_name, long timestamp);

/**
 * @brief 设置key的过期时间,以毫秒计
 * @return int
 * @retval -1:执行失败; 0:key不存在,或设置失败; 1:设置成功
**/
int ads_redis_pexpire(redisContext *redis_ptr, const char *key_name, long milliseconds);

/**
 * @brief 设置key过期的时间戳,以毫秒计
 * @return int
 * @retval -1:执行失败; 0:key不存在,或设置失败; 1:设置成功
**/
int ads_redis_pexpireat(redisContext *redis_ptr, const char *key_name, long millitimestamp);

// String相关操作 //

/**
 * @brief 设置key的值
 * @return int
 * @retval -1:执行失败; 0:设置成功
**/
int ads_redis_set(redisContext *redis_ptr, const char *key_name, const char *str);

/**
 * @brief 设置key的值(二进制)
 * @return int
 * @retval -1:执行失败; 0:设置成功
**/
int ads_redis_setb(redisContext *redis_ptr, const char *key_name, const char *str, int len);

/**
 * @brief 获取指定key的值
 * @return int
 * @retval -1:执行失败; 0:不存在; >0:长度
**/
int ads_redis_get(redisContext *redis_ptr, const char *key_name, char *str, int maxsize);

// Set相关操作 //

/**
 * @brief 将一个成员加入到集合中,集合不存在则创建
 * @return int
 * @retval -1:执行失败; 0:成员已存在; 1:加入成功
**/
int ads_redis_sadd(redisContext *redis_ptr, const char *key_name, const char *member);

/**
 * @brief 判断是否是集合的成员
 * @return int
 * @retval -1:执行失败; 0:不存在; 1:存在
**/
int ads_redis_sismember(redisContext *redis_ptr, const char *key_name, const char *member);

/**
 * @brief 获取集合的成员数
 * @return int
 * @retval -1:执行失败; 0:集合不存在
**/
int ads_redis_scard(redisContext *redis_ptr, const char *key_name);

/**
 * @brief 移除集合中的一个成员
 * @return int
 * @retval -1:执行失败; 0:成员不存在; 1:移除成功
**/
int ads_redis_srem(redisContext *redis_ptr, const char *key_name, const char *member);

#endif
