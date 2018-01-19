#include "core/monitor/ads_monitor_service.h"

#include "log.h"
#include "core/monitor/ads_monitor_param.h"
#include "core/monitor/ads_monitor_manager.h"


AdsMonitorService::AdsMonitorService()
{

}

AdsMonitorService::~AdsMonitorService()
{

}

bool AdsMonitorService::init()
{
	_manager = new (std::nothrow) AdsMonitorManager;
	if (_manager == NULL) {
		WARN("monitor manager is null");
		return false;
	}

	return true;
}


/**
 * 查询接口
 */
unsigned int AdsMonitorService::getCampaignTotalCost(int id)
{
	return 0;
}
unsigned int AdsMonitorService::getCampaignTotalImp(int id)
{
	return 0;
}
unsigned int AdsMonitorService::getCampaignTotalClk(int id)
{
	return 0;
}

unsigned int AdsMonitorService::getCampaignImpFreq(int id, 
	const string& devid, time_t ts)
{
	return 0;
}
unsigned int AdsMonitorService::getCampaignClkFreq(int id,
	const string& devid, time_t ts)
{
	return 0;
}

unsigned int AdsMonitorService::getLaunchTodayCost(int id)
{
	return 0;
}
unsigned int AdsMonitorService::getLaunchTodayImp(int id)
{
	return 0;
}
unsigned int AdsMonitorService::getLaunchTodayClk(int id)
{
	return 0;
}

unsigned int AdsMonitorService::getLaunchImpFreq(int id, 
	const string& devid, time_t ts)
{
	return 0;
}
unsigned int AdsMonitorService::getLaunchClkFreq(int id, 
	const string& devid, time_t ts)
{
	return 0;
}

/**
 * 更新接口
 */
void AdsMonitorService::update(const AdsMonitorParam* param)
{

}