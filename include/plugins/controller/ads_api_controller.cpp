#include "plugins/controller/ads_api_controller.h"

#include <string>
#include <iostream>
#include "log.h"
#include "core/advertise/ads_advertise_service.h"
#include "core/advertise/ads_advertise_manager.h"
#include "core/monitor/ads_monitor_types.h"
#include "core/monitor/ads_monitor_manager.h"
#include "core/monitor/ads_monitor_service.h"
#include "utils/ads_string.h"

using std::string;
using std::cout;
using std::endl;

static AdsAdvertiseCollection* getAdCollection();
static AdsMonitorManager* getMonitorManager();

static void act_list(AdsThreadData* pdata);
static void act_view(AdsThreadData* pdata);

static void view_campaign(AdsCampaign *campaign);
static void view_monitor_db(AdsMonitorDataBase *db);

int AdsApiController::process(AdsThreadData* pdata)
{
	AdsHttpRequest *request = pdata->request;
	string arg1 = request->getUri(1);
	if ( arg1 == "list" ) {
		act_list(pdata);
	} else if ( arg1 == "view" ) {
		act_view(pdata);
	}

	return ADS_HTTP_OK;
}

static AdsAdvertiseCollection* getAdCollection()
{
	AdsAdvertiseService& adServ = getAdvertiseService();
	AdsAdvertiseManager *manager = AdsAdvertiseService::Api::getManager(&adServ);
	return manager->get();
}

static AdsMonitorManager* getMonitorManager()
{
	AdsMonitorService& monitorServ = getMonitorService();
	return AdsMonitorService::Api::getManager(&monitorServ);
}

static void act_list(AdsThreadData* pdata)
{
	AdsHttpRequest *request = pdata->request;
	string arg2 = request->getUri(2);
	if ( arg2 == "campaign" ) {
		AdsAdvertiseCollection *collection = getAdCollection();
		list<AdsCampaign*> l;
		collection->getAllCampaigns(l);
		for ( auto cam : l ) {
			cout << cam->id << endl;
		}
	} 
}

static void act_view(AdsThreadData* pdata)
{
	AdsHttpRequest *request = pdata->request;
	string arg2 = request->getUri(2);

	AdsAdvertiseCollection *collection = getAdCollection();
	AdsMonitorManager *monitorManager = getMonitorManager();

	if ( arg2 == "campaign" ) {
		int id = ads_string_to_int( request->getUri(3) );
		AdsCampaign *campaign = collection->getCampaign(id);
		if (campaign == NULL) {
			cout << "campaign is not exists" << endl;
			return;
		}
		view_campaign(campaign);
		AdsMonitorDataBase *db = monitorManager->findCampaignData(id);
		view_monitor_db(db);

	} else if ( arg2 == "launch" ) {
		int id = ads_string_to_int( request->getUri(3) );
		AdsLaunch *launch = collection->getLaunch(id);
		if (launch == NULL) {
			cout << "launch is not exists" << endl;
			return;
		}
		view_campaign(launch);
		AdsMonitorDataBase *db = monitorManager->findLaunchData(id);
		view_monitor_db(db);
	}
}

static void view_campaign(AdsCampaign *campaign)
{

}

static void view_monitor_db(AdsMonitorDataBase *db)
{
	if ( db == NULL ) {
		cout << "no data" << endl;
		return;
	}

	cout << "Today:" << endl;
	cout << "imp \t clk \t cost" << endl;
	cout << db->getImpTotal() << "\t";
	cout << db->getClkTotal() << "\t";
	cout << db->getCostTotal() << "\t";
	cout << endl;

	cout << "Imp Record:" << endl;
	AdsMonitorRecord& ir = db->getImpRecord();
	for ( auto& itr : ir ) {
		cout << itr.first << "\t" << endl;
		for ( auto t : *(itr.second) ) {
			cout << "\t" << t << endl;
		}
	}
}