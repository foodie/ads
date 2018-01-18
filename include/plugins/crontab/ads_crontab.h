#ifndef _ADS_CRONTAB_H
#define _ADS_CRONTAB_H

#include <string>

using std::string;

class AdsCrontabTask;

class AdsCrontab : public AdsSingleton<AdsCrontab>
{
	friend class AdsSingleton<AdsCrontab>;
public:
	~AdsCrontab();

	bool init(); 
	void process();

	void add(const string& format, AdsCrontabTask *task);

private:
	AdsCrontab();	
};

static inline void registerCrontabTask(const string& format, AdsCrontabTask *task)
{
	AdsCrontab::getInstance().add(format, task);
}

#endif
/* vim: set ts=4 sw=4 noet: */
 