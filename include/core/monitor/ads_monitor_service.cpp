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
	AdsMonitorDataBase *data = _manager->findCampaignData(id);
	if ( data == NULL ) {
		return 0;
	}
	return data->getCostTotal();
}
unsigned int AdsMonitorService::getCampaignTotalImp(int id)
{
	AdsMonitorDataBase *data = _manager->findCampaignData(id);
	if ( data == NULL ) {
		return 0;
	}
	return data->getImpTotal();
}
unsigned int AdsMonitorService::getCampaignTotalClk(int id)
{
	AdsMonitorDataBase *data = _manager->findCampaignData(id);
	if ( data == NULL ) {
		return 0;
	}
	return data->getClkTotal();
}

unsigned int AdsMonitorService::getCampaignImpFreq(int id, 
	const string& devid, time_t ts)
{
	AdsMonitorDataBase *data = _manager->findCampaignData(id);
	if ( data == NULL ) {
		return 0;
	}
	return data->getImpRecordNum(devid, ts);
}
unsigned int AdsMonitorService::getCampaignClkFreq(int id,
	const string& devid, time_t ts)
{	
	AdsMonitorDataBase *data = _manager->findCampaignData(id);
	if ( data == NULL ) {
		return 0;
	}
	return data->getClkRecordNum(devid, ts);
}

unsigned int AdsMonitorService::getLaunchTodayCost(int id)
{
	AdsMonitorDataBase *data = _manager->findLaunchData(id);
	if ( data == NULL ) {
		return 0;
	}
	return data->getCostToday();
}
unsigned int AdsMonitorService::getLaunchTodayImp(int id)
{
	AdsMonitorDataBase *data = _manager->findLaunchData(id);
	if ( data == NULL ) {
		return 0;
	}
	return data->getImpToday();
}
unsigned int AdsMonitorService::getLaunchTodayClk(int id)
{
	AdsMonitorDataBase *data = _manager->findLaunchData(id);
	if ( data == NULL ) {
		return 0;
	}
	return data->getClkToday();
}

unsigned int AdsMonitorService::getLaunchImpFreq(int id, 
	const string& devid, time_t ts)
{
	AdsMonitorDataBase *data = _manager->findLaunchData(id);
	if ( data == NULL ) {
		return 0;
	}
	return data->getImpRecordNum(devid, ts);
}
unsigned int AdsMonitorService::getLaunchClkFreq(int id, 
	const string& devid, time_t ts)
{
	AdsMonitorDataBase *data = _manager->findLaunchData(id);
	if ( data == NULL ) {
		return 0;
	}
	return data->getClkRecordNum(devid, ts);
}

/**
 * 更新接口
 */
void AdsMonitorService::update(const AdsMonitorParam* param)
{

}