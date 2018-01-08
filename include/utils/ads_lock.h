#ifndef _ADS_LOCK_H
#define _ADS_LOCK_H

#include <pthread.h>

// 互斥锁监听
class AdsMutexlockMonitor
{
public:
	AdsMutexlockMonitor(pthread_mutex_t *lock) : _lock(lock) {
		pthread_mutex_lock(_lock);
	}
	~AdsMutexlockMonitor() {
		pthread_mutex_unlock(_lock);
	}

private:
	pthread_mutex_t *_lock;
};

// 读写锁监听
enum {
	RWLOCK_MODE_READ  = 0x01,	// 读锁
	RWLOCK_MODE_WRITE = 0x02,	// 写锁
};
class AdsRwlockMonitor
{
public:
	AdsRwlockMonitor(pthread_rwlock_t *lock, int mode) : _lock(lock) {
		// 1-读 2-写
		if (mode == (int) RWLOCK_MODE_WRITE) {
			pthread_rwlock_wrlock(_lock);
		} else {
			pthread_rwlock_rdlock(_lock);
		}
	}
	~AdsRwlockMonitor() {
		pthread_rwlock_unlock(_lock);
	}
	
private:
	pthread_rwlock_t *_lock;
};


#endif
/* vim: set ts=4 sw=4 noet: */
