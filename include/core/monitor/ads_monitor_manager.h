#ifndef _ADS_MONITOR_MANAGER_H
#define _ADS_MONITOR_MANAGER_H

/**
 * 监测数据管理
 */

#include "monitor/ads_monitor_types.h"

class AdsMonitorManager
{
public:
	AdsMonitorManager() 
		: campaigns(MAX_MONITOR_BITEMS_SIZE), 
		  launchs(MAX_MONITOR_BITEMS_SIZE), 
		  creatives(MAX_MONITOR_BITEMS_SIZE) 
	{}

	

	AdsMonitorCampaign* getCampaign(int id);
	AdsMonitorLaunch* getLaunch(int id);
	AdsMonitorCreative* getCreative(int id);

private:
	phashmap<int, AdsMonitorCampaign*> campaigns;
	phashmap<int, AdsMonitorLaunch*> launchs;
	phashmap<int, AdsMonitorCreative*> creatives;
};


#endif
/* vim: set ts=4 sw=4 noet: */
