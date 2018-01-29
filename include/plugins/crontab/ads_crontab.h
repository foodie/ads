#ifndef _ADS_CRONTAB_H
#define _ADS_CRONTAB_H

#include <string>
#include <utility>
#include <list>
#include "core/ads_singleton.h"

using std::string;
using std::pair;
using std::list;

class AdsCrontabTask;

class AdsCrontab : public AdsSingleton<AdsCrontab>
{
	friend class AdsSingleton<AdsCrontab>;
public:
	typedef pair<string, AdsCrontabTask*> _Tasker;

	~AdsCrontab();

	bool init(); 
	void process();

	void add(const string& format, AdsCrontabTask *task);

private:
	AdsCrontab();
	list<_Tasker> _tasks;

	bool checkTime(const string& format, time_t ts);
};

static inline bool initCrontab()
{
	return AdsCrontab::getInstance().init();
}

static inline void registerCrontabTask(const string& format, AdsCrontabTask *task)
{
	AdsCrontab::getInstance().add(format, task);
}

#endif
/* vim: set ts=4 sw=4 noet: */
 