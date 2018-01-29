#include "plugins/crontab/ads_crontab.h"

#include <tr1/functional>
#include <sys/time.h>
#include "log.h"
#include "utils/ads_time.h"
#include "plugins/crontab/ads_crontab_task.h"


static void crontab_handler(int arg)
{
	AdsCrontab& crontab = AdsCrontab::getInstance();
	crontab.process();
}

AdsCrontab::AdsCrontab()
{

}

AdsCrontab::~AdsCrontab()
{
	for ( auto itr = tasks.begin(); itr != tasks.end(); itr++ ) {
		delete itr->second;
	}
	tasks.clear();
}

bool AdsCrontab::init()
{

/*
    if (pthread_create(&master, NULL, crontabprocess, this)) {
        WARN("[Crontab] create master thread failed");
        return -1;
    }
*/

	time_t nowtime = ads_nowtime();
    struct itimerval t;
    t.it_interval.tv_usec = 0;
    t.it_interval.tv_sec = 60;
    t.it_value.tv_usec = 0;
    t.it_value.tv_sec = 60 - nowtime % 60;
    if ( setitimer( ITIMER_REAL, &t, NULL ) ) {
        WARN("[Crontab] settimer failed");
        return false;
    }
    signal( SIGALRM, crontab_handler );

	INFO("[Crontab] Crontab plugins init success");
	return true;
}

void AdsCrontab::process()
{

	INFO("[Crontab] master process run success");
}

void AdsCrontab::add(const string& format, AdsCrontabTask *task)
{
	
}