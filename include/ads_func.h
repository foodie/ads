#ifndef _ADS_FUNC_H
#define _ADS_FUNC_H

#include <pthread.h>
#include <string>

std::string seconds_to_his(int seconds); // 秒数转换为时分秒, 15 => 00:00:15

// device
void ads_imei_md5(const char *imei, char *imei_md5);
void ads_androidid_md5(const char *androidid, char *androidid_md5);
void ads_mac_md5(const char *mac, char *mac_md5);
void ads_mac_to_mac1(const char *mac, char *mac1);
void ads_mac1_to_mac(const char *mac1, char *mac);

uint32_t ads_ip_to_long(const char *ip);
void 	 ads_long_to_ip(uint32_t l, char *ip);

// 互斥锁监听
class mutexlock_monitor
{
	pthread_mutex_t *_lock;
public:
	mutexlock_monitor(pthread_mutex_t *lock) : _lock(lock) {
		pthread_mutex_lock(_lock);
	}
	~mutexlock_monitor() {
		pthread_mutex_unlock(_lock);
	}
};

// 读写锁监听
enum {
	RWLOCK_MODE_READ  = 0x01,	// 读锁
	RWLOCK_MODE_WRITE = 0x02,	// 写锁
};
class rwlock_monitor
{
	pthread_rwlock_t *_lock;
public:
	rwlock_monitor(pthread_rwlock_t *lock, int mode) : _lock(lock) {
		// 1-读 2-写
		if (mode == (int) RWLOCK_MODE_WRITE) {
			pthread_rwlock_wrlock(_lock);
		} else {
			pthread_rwlock_rdlock(_lock);
		}
	}
	~rwlock_monitor() {
		pthread_rwlock_unlock(_lock);
	}
};

#endif
/* vim: set ts=4 sw=4 noet: */
