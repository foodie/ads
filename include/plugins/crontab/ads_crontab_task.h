#ifndef _ADS_CRONTAB_TASK_H
#define _ADS_CRONTAB_TASK_H

class AdsCrontabTime
{
	int second;
	int minute;
	int hour;
	int day;
	int week;
};

class AdsCrontabTask
{
public:
	virtual void run() = 0;
};

#endif
/* vim: set ts=4 sw=4 noet: */
