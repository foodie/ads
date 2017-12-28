#include "ads_redis.h"

/**
 * @brief 连接redis数据库
 * @return int
 * @retval -1:连接失败; 0:连接成功
**/
int ads_redis_connect(redisContext **redis_ptr, const char *host, int port)
{
	*redis_ptr = redisConnect(host, port);
	if (*redis_ptr == NULL) {
		return -1;
	}
	if ((*redis_ptr)->err) {
		redisFree(*redis_ptr);
		return -1;
	}
	return 0;	
}

/**
 * @brief 切换数据库
 * @return int
 * @retval -1:操作失败; 0:操作成功
**/
int ads_redis_select(redisContext *redis_ptr, int index)
{
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "select %d", index);
	if (reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK") == 0) {
		freeReplyObject(reply);
		return 0;
	}
	freeReplyObject(reply);
	return -1;
}

/**
 * @brief key存在时删除key
 * @return int
 * @retval -1:执行失败; 0:key不存在; 1:删除成功
**/
int ads_redis_del(redisContext *redis_ptr, const char *key_name)
{
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "del %s", key_name);
	if (reply->type == REDIS_REPLY_INTEGER) {
		int ret = reply->integer;
		freeReplyObject(reply);
		return ret;
	}
	freeReplyObject(reply);
	return -1;
}

/**
 * @brief key是否存在
 * @return int
 * @retval -1:执行失败; 0:不存在; 1:存在
**/
int ads_redis_exists(redisContext *redis_ptr, const char *key_name)
{
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "exists %s", key_name);
	if (reply->type == REDIS_REPLY_INTEGER) {
		int ret = reply->integer;
		freeReplyObject(reply);
		return ret;
	}
	freeReplyObject(reply);
	return -1;
}

/**
 * @brief 修改key的名称,若new_key已经存在,则覆盖旧值
 * @return int
 * @retval -1:执行失败; 0:执行成功
**/
int ads_redis_rename(redisContext *redis_ptr, const char *key_name, const char *new_key_name)
{
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "rename %s %s", key_name, new_key_name);
	if (reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK") == 0) {
		freeReplyObject(reply);
		return 0;
	}
	freeReplyObject(reply);
	return -1;
}

/**
 * @brief 当new_key不存在时修改key的名称
 * @return int
 * @retval -1:执行失败; 0:new_key存在,rename失败; 1:new_key不存在,rename成功
**/
int ads_redis_renamenx(redisContext *redis_ptr, const char *key_name, const char *new_key_name)
{
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "renamenx %s %s", key_name, new_key_name);
	if (reply->type == REDIS_REPLY_INTEGER) {
		int ret = reply->integer;
		freeReplyObject(reply);
		return ret;
	}
	freeReplyObject(reply);
	return -1;
}

/**
 * @brief 设置key的过期时间
 * @return int
 * @retval -1:执行失败; 0:key不存在,或设置失败; 1:设置成功
**/
int ads_redis_expire(redisContext *redis_ptr, const char *key_name, long seconds)
{
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "expire %s %ld", key_name, seconds);
	if (reply->type == REDIS_REPLY_INTEGER) {
		int ret = reply->integer;
		freeReplyObject(reply);
		return ret;
	}
	freeReplyObject(reply);
	return -1;
}

/**
 * @brief 设置key过期的时间戳
 * @return int
 * @retval -1:执行失败; 0:key不存在,或设置失败; 1:设置成功
**/
int ads_redis_expireat(redisContext *redis_ptr, const char *key_name, long timestamp)
{
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "expireat %s %ld", key_name, timestamp);
	if (reply->type == REDIS_REPLY_INTEGER) {
		int ret = reply->integer;
		freeReplyObject(reply);
		return ret;
	}
	freeReplyObject(reply);
	return -1;
}

/**
 * @brief 设置key的过期时间,以毫秒计
 * @return int
 * @retval -1:执行失败; 0:key不存在,或设置失败; 1:设置成功
**/
int ads_redis_pexpire(redisContext *redis_ptr, const char *key_name, long milliseconds)
{
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "pexpire %s %ld", key_name, milliseconds);
	if (reply->type == REDIS_REPLY_INTEGER) {
		int ret = reply->integer;
		freeReplyObject(reply);
		return ret;
	}
	freeReplyObject(reply);
	return -1;
}

/**
 * @brief 设置key过期的时间戳,以毫秒计
 * @return int
 * @retval -1:执行失败; 0:key不存在,或设置失败; 1:设置成功
**/
int ads_redis_pexpireat(redisContext *redis_ptr, const char *key_name, long millitimestamp)
{
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "pexpireat %s %ld", key_name, millitimestamp);
	if (reply->type == REDIS_REPLY_INTEGER) {
		int ret = reply->integer;
		freeReplyObject(reply);
		return ret;
	}
	freeReplyObject(reply);
	return -1;	
}

// String相关操作 //
/**
 * @brief 设置key的值
 * @return int
 * @retval -1:执行失败; 0:设置成功
**/
int ads_redis_set(redisContext *redis_ptr, const char *key_name, const char *str)
{
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "set %s %s", key_name, str);
	if (reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK") == 0) {
		freeReplyObject(reply);
		return 0;
	}
	freeReplyObject(reply);
	return -1;
}

/**
 * @brief 设置key的值(二进制)
 * @return int
 * @retval -1:执行失败; 0:设置成功
**/
int ads_redis_setb(redisContext *redis_ptr, const char *key_name, const char *str, int len)
{	
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "set %s %b", key_name, str, len);
	if (reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK") == 0) {
		freeReplyObject(reply);
		return 0;
	}
	freeReplyObject(reply);
	return -1;	
}

/**
 * @brief 获取指定key的值
 * @return int
 * @retval -1:执行失败; 0:不存在; 1:获取成功
**/
int ads_redis_get(redisContext *redis_ptr, const char *key_name, char *str, int maxsize)
{
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "get %s", key_name);
	if (reply->type == REDIS_REPLY_STRING) {
		int len = reply->len > maxsize ? maxsize : reply->len;
		memcpy(str, reply->str, len);
		freeReplyObject(reply);
		return len;
	}
	if (reply->type == REDIS_REPLY_NIL) {
		freeReplyObject(reply);
		return 0;
	}
	freeReplyObject(reply);
	return -1;
}

// Set相关操作 //

/**
 * @brief 将一个成员加入到集合中,集合不存在则创建
 * @return int
 * @retval -1:执行失败; 0:成员已存在; 1:加入成功
**/
int ads_redis_sadd(redisContext *redis_ptr, const char *key_name, const char *member)
{
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "sadd %s %s", key_name, member);
	if (reply->type == REDIS_REPLY_INTEGER) {
		int ret = reply->integer;
		freeReplyObject(reply);
		return ret;
	}
	freeReplyObject(reply);
	return -1;
}

/**
 * @brief 判断是否是集合的成员
 * @return int
 * @retval -1:执行失败; 0:不存在; 1:存在
**/
int ads_redis_sismember(redisContext *redis_ptr, const char *key_name, const char *member)
{
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "sismember %s %s", key_name, member);
	if (reply->type == REDIS_REPLY_INTEGER) {
		int ret = reply->integer;
		freeReplyObject(reply);
		return ret;
	}
	freeReplyObject(reply);
	return -1;
}

/**
 * @brief 获取集合的成员数
 * @return int
 * @retval -1:执行失败
**/
int ads_redis_scard(redisContext *redis_ptr, const char *key_name)
{
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "scard %s", key_name);
	if (reply->type == REDIS_REPLY_INTEGER) {
		int ret = reply->integer;
		freeReplyObject(reply);
		return ret;
	}
	freeReplyObject(reply);
	return -1;
}

/**
 * @brief 移除集合中的一个成员
 * @return int
 * @retval -1:执行失败; 0:成员不存在; 1:移除成功
**/
int ads_redis_srem(redisContext *redis_ptr, const char *key_name, const char *member)
{
	if (redis_ptr == 0) {
		return -1;
	}
	redisReply* reply = (redisReply*) redisCommand(redis_ptr, "srem %s %s", key_name, member);
	if (reply->type == REDIS_REPLY_INTEGER) {
		int ret = reply->integer;
		freeReplyObject(reply);
		return ret;
	}
	freeReplyObject(reply);
	return -1;
}
