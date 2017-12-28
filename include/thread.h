#ifndef _THREAD_H
#define _THREAD_H

#include <pthread.h>
#include "log.h"

class Thread
{
	Thread(const Thread &);
	void operator=(const Thread &);
	pthread_t _thread;

public:
	Thread() : _thread(0), _status(true), _start(false) {}
	virtual ~ Thread() {}

    //启动线程函数
    int start()
    {
        int ret = 0;
        if (_start == false) {
            ret = pthread_create(&_thread, NULL, Thread::func, this);
            if (ret != 0) {
                FATAL("create thread failed.");
                _status = false;
            } else {
                _start = true;
            }
        }
        return ret;
    }
    
    //取得线程的状态
    bool valid() const
    {
        return _status;
    }
    
    //取得线程句柄
    pthread_t & get()
    {
        return _thread;
    }

    const pthread_t & get() const
    {
        return _thread;
    }

    //等待线程结束函数
    int join()
    {
        if (_status == true && _start == true) {
            return pthread_join(_thread, NULL);
        }
        return -1;
    }

	virtual void *run() = 0;

protected:
    bool _status;
    bool _start;

    static void *func(void *args)
    {
        Thread *p_thread = static_cast <Thread *>(args);
        return p_thread->run();
    }
};

#endif
/* vim: set ts=4 sw=4 noet: */
