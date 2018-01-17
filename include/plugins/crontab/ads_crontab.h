#ifndef _ADS_CRONTAB_H
#define _ADS_CRONTAB_H

#include <string>

using std::string;

class AdsCrontabTask;

class AdsCrontab
{
public:
	virtual int process() = 0;

	static void add(const string& format, AdsCrontabTask *task);
};

#endif
/* vim: set ts=4 sw=4 noet: */
 