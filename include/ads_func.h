#ifndef _ADS_FUNC_H
#define _ADS_FUNC_H

#include <pthread.h>
#include <string>

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
