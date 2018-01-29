#ifndef _ADS_MONITOR_CRONTAB_H
#define _ADS_MONITOR_CRONTAB_H

#include "plugins/crontab/ads_crontab_task.h"
#include "core/monitor/ads_monitor_service.h"

/**
 * @brief      Class for ads advertise reload task.
 */
class AdsAdvertiseRecycleTask : public AdsCrontabTask
{
public:
	virtual void run() override
	{
		AdsMonitorService& mServ = getMonitorService();
		

		INFO("[Monitor] Monitor recycle data success");
	}
};

#endif
/* vim: set ts=4 sw=4 noet: */
