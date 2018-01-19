#ifndef _ADS_MONITOR_MANAGER_H
#define _ADS_MONITOR_MANAGER_H

/**
 * 监测数据管理
 */
#include "phashmap.h"
#include "core/monitor/ads_monitor_types.h"
#include "core/monitor/ads_monitor_loader.h"

using lib::phashmap;

class AdsMonitorManager
{
public:
	AdsMonitorManager() 
		: campaigns(MAX_MONITOR_BITEMS_SIZE), 
		  launchs(MAX_MONITOR_BITEMS_SIZE), 
		  //creatives(MAX_MONITOR_BITEMS_SIZE) 
	{}

	void loadHistoryData();
	void getLogData();

	AdsMonitorDataBase* addCampaignData(int id)
	{
		add(campaigns, id);
	}
	AdsMonitorDataBase* findCampaignData(int id)
	{
		find(campaigns, id);
	}
	AdsMonitorDataBase* getCampaignData(int id)
	{
		get(campaigns, id);
	}

	AdsMonitorDataBase* addLaunchData(int id)
	{
		add(launchs, id);
	}
	AdsMonitorDataBase* findLaunchData(int id)
	{
		find(launchs, id);
	}
	AdsMonitorDataBase* getLaunchData(int id)
	{
		get(launchs, id);
	}

private:
	phashmap<int, AdsMonitorDataBase*> campaigns;
	phashmap<int, AdsMonitorDataBase*> launchs;
	//phashmap<int, AdsMonitorDataBase*> creatives;

	AdsMonitorDataBase* find(phashmap<int, AdsMonitorDataBase*>& src, int id)
	{
		AdsMonitorDataBase* v;
		if ( src.get(id, &v) == lib::HASH_NOEXIST ) {
			return NULL;
		}
		return v;
	}

	AdsMonitorDataBase* add(phashmap<int, AdsMonitorDataBase*>& src, int id)
	{
		AdsMonitorDataBase *v = new (std::nothrow) AdsMonitorDataBase;
		src.set(id, v);
		return v;
	}

	AdsMonitorDataBase* get(phashmap<int, AdsMonitorDataBase*>& src, int id)
	{
		AdsMonitorDataBase* v = find(src, id);
		if ( v == NULL) {
			v = add(src, id);
		}
		return v;
	}

};


#endif
/* vim: set ts=4 sw=4 noet: */
