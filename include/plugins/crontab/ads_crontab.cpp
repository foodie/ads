#include "plugins/crontab/ads_crontab.h"

#include <tr1/functional>
#include "log.h"
#include "plugins/crontab/ads_crontab_task.h"


static void* crontabprocess(void *arg)
{
	((AdsCrontab*) arg)->process();
	return NULL;
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

    if (pthread_create(&master, NULL, crontabprocess, this)) {
        WARN("[Crontab] create master thread failed");
        return -1;
    }

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