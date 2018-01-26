#include "plugins/crontab/ads_crontab.h"

#include <tr1/functional>
#include "log.h"

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
	std::tr1::function<void(void*)> f = std::tr1::bind(&AdsCrontab::process, 
		this, std::tr1::placeholders::_1);
    if (pthread_create(&master, NULL, f, NULL)) {
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