#include "core/monitor/ads_monitor_manager.h"

/**
 * Campaign
 */

bool AdsMonitorManager::setCampaign(int id, AdsMonitorCampaign *v)
{
	AdsMonitorCampaign *v2 = findCampaign(id);
	int ret = campaigns.set(id, v, 1);
	if ( ret == lib::HASH_INSERT_SEC || ret == lib::HASH_OVERWRITE ) {
		delete v2;
		return true;
	}
	return false;
}

AdsMonitorCampaign* AdsMonitorManager::findCampaign(int id)
{
	AdsMonitorCampaign* v;
	if ( campaigns.get(id, &v) == lib::HASH_NOEXIST ) {
		return NULL;
	}
	return v;
}

AdsMonitorCampaign* AdsMonitorManager::getCampaign(int id)
{
	AdsMonitorCampaign* v = findCampaign(id);
	if ( v == NULL ) {
		v = new (std::nothrow) AdsMonitorCampaign;
		campaigns.set(id, v);
	}
	return v;
}

/**
 * Launch
 */

bool AdsMonitorManager::setLaunch(int id, AdsMonitorLaunch *v)
{
	AdsMonitorLaunch *v2 = findLaunch(id);
	int ret = launchs.set(id, v, 1);
	if ( ret == lib::HASH_INSERT_SEC || ret == lib::HASH_OVERWRITE ) {
		delete v2;
		return true;
	}
	return false;
}

AdsMonitorLaunch* AdsMonitorManager::findLaunch(int id)
{
	AdsMonitorLaunch* v;
	if ( launchs.get(id, &v) == lib::HASH_NOEXIST ) {
		return NULL;
	}
	return v;
}

AdsMonitorLaunch* AdsMonitorManager::getLaunch(int id)
{
	AdsMonitorLaunch* v = findLaunch(id);
	if ( v == NULL ) {
		v = new (std::nothrow) AdsMonitorLaunch;
		launchs.set(id, v);
	}
	return v;
}
