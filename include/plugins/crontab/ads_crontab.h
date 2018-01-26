#ifndef _ADS_CRONTAB_H
#define _ADS_CRONTAB_H

#include <string>
#include <list>
#include "core/ads_singleton.h"

using std::string;
using std::list;

class AdsCrontabTask;

class AdsCrontab : public AdsSingleton<AdsCrontab>
{
	friend class AdsSingleton<AdsCrontab>;
public:
	typedef pair<string, AdsCrontabTask*> _Task;

	~AdsCrontab();

	bool init(); 
	void process();

	void add(const string& format, AdsCrontabTask *task);

private:
	AdsCrontab();
	list<_Task> tasks;

	pthread_t master;
};

static inline void registerCrontabTask(const string& format, AdsCrontabTask *task)
{
	AdsCrontab::getInstance().add(format, task);
}

#endif
/* vim: set ts=4 sw=4 noet: */
 