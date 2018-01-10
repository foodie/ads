#include "monitor/ads_monitor_service.h"

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
	
}


void AdsMonitorService::addCampaignWin(int id, int price)
{
	AdsMonitorCampaign* v;
	if ( manager->getCampaign(id) == NULL ) {
		v = new (std::nothrow) AdsMonitorCampaign();
		manager->setCampaign(id, v);
	}

	v->incCost(price);

}

void AdsMonitorService::addCampaignImp(int id, const string& name)
{

}

void AdsMonitorService::addCampaignClk(int id, const string& name);
