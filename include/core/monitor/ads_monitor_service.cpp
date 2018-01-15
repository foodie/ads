#include "core/monitor/ads_monitor_service.h"

#include "log.h"

AdsMonitorManager* AdsMonitorService::manager = new (std::nothrow) AdsMonitorManager();


bool AdsMonitorService::init()
{
	if (manager == NULL) {
		WARN("monitor manager is null");
		return false;
	}

	return true;
}

void AdsMonitorService::close()
{

}


static bool checkCampaignBudget(int id, int budget)
{
	return true;
}

/**
 * 更新接口
 */

void AdsMonitorService::addCampaignCost(int id, int price)
{
	AdsMonitorCampaign* v = manager->getCampaign(id);
	if ( v != NULL ) {
		v->incCost(price);
	}
}

void AdsMonitorService::addCampaignImp(int id, const string& name)
{
	AdsMonitorCampaign* v = manager->getCampaign(id)
	if ( v != NULL ) {
		v->incImp(1);
		v->addImpRecord(name, ads_nowtime());
	}
}

void AdsMonitorService::addCampaignClk(int id, const string& name)
{
	AdsMonitorCampaign* v = manager->getCampaign(id)
	if ( v != NULL ) {
		v->incClk(1);
		v->addClkRecord(name, ads_nowtime());
	}
}

void AdsMonitorService::addLaunchCost(int id, int price)
{
	AdsMonitorLaunch* v = manager->getLaunch(id)
	if ( v != NULL ) {
		v->incCost(price);
	}
}

void AdsMonitorService::addLaunchImp(int id, const string& name)
{
	AdsMonitorLaunch* v = manager->getLaunch(id)
	if ( v != NULL ) {
		v->incImp(1);
		v->addImpRecord(name, ads_nowtime());
	}
}

void AdsMonitorService::addLaunchClk(int id, const string& name)
{
	AdsMonitorLaunch* v = manager->getLaunch(id)
	if ( v != NULL ) {
		v->incClk(1);
		v->addClkRecord(name, ads_nowtime());
	}
}