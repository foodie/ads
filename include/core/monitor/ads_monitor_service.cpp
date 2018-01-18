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


static bool checkCampaignBudget(int id, int budget)
{
	return true;
}


/**
 * 查询接口
 */
int AdsMonitorService::getCampaignCost(int id)
{
	return 0;
}
int AdsMonitorService::getCampaignImp(int id)
{
	return 0;
}
int AdsMonitorService::getCampaignClk(int id)
{
	return 0;
}
int AdsMonitorService::getCampaignImpFreq(int id)
{
	return 0;
}
int AdsMonitorService::getCampaignClkFreq(int id)
{
	return 0;
}

int AdsMonitorService::getLaunchCost(int id)
{
	return 0;
}
int AdsMonitorService::getLaunchImpFreq(int id)
{
	return 0;
}
int AdsMonitorService::getLaunchClkFreq(int id)
{
	return 0;
}

/**
 * 更新接口
 */
void AdsMonitorService::update(const AdsMonitorParam* param)
{

}