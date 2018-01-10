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

	void getHistoryData();
	void getLogData();

	bool setCampaign(int id, AdsMonitorCampaign *v);
	AdsMonitorCampaign* findCampaign(int id);
	AdsMonitorCampaign* getCampaign(int id);

	bool setLaunch(int id, AdsMonitorLaunch *v);
	AdsMonitorLaunch* findLaunch(int id);
	AdsMonitorLaunch* getLaunch(int id);


private:
	phashmap<int, AdsMonitorCampaign*> campaigns;
	phashmap<int, AdsMonitorLaunch*> launchs;
};


#endif
/* vim: set ts=4 sw=4 noet: */
