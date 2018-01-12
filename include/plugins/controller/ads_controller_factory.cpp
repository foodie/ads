#include "plugins/controller/ads_controller_factory.h"

#include "log.h"
#include "utils/ads_string.h"
#include "plugins/controller/ads_bidding_controller.h"
#include "plugins/controller/ads_monitor_controller.h"

unordered_map<string, AdsController*>* AdsControllerFactory::controllers = new unordered_map<string, AdsController*>;

bool AdsControllerFactory::init()
{
	if ( controllers == NULL ) {
		WARN("Controllers is null");
		return false;
	}

	// bidding
	AdsBiddingController *bidding = new AdsBiddingController;
	if ( bidding == NULL ) {
		WARN("Bidding controller is null");
		return false;
	}

	// monitor
	AdsMonitorController *monitor = new AdsMonitorController;
	if ( monitor == NULL ) {
		WARN("Monitor controller is null");
		return false;
	}

	return true;
}

AdsController* AdsControllerFactory::getController(const string &name)
{
	if ( controllers != NULL ) {
		ads_string_tolower(name);
		auto itr = controllers->find(name);
		if ( itr != controllers->end() ) {
			return itr->second;
		}
	}
	return NULL;
}