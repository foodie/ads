#include "plugins/controller/ads_api_controller.h"

#include <string>
#include <iostream>
#include "log.h"
#include "core/advertise/ads_advertise_service.h"
#include "core/monitor/ads_monitor_service.h"

using std::string;
using std::cout;
using std::endl;

static AdsAdvertiseCollection* getAdCollection();

static void act_list(AdsThreadData* pdata);
static void act_view(AdsThreadData* pdata);

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
	AdsAdvertiseService::Api ap(&adServ);
	AdsAdvertiseManager *manager = ap.getManager();
	return manager->get();
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
			
		}
	} 
}

static void act_view(AdsThreadData* pdata)
{
	AdsHttpRequest *request = pdata->request;
	string arg2 = request->getUri(2);
	if ( arg2 == "campaign" ) {
			
	} else if ( arg2 == "launch" ) {

	}
}
