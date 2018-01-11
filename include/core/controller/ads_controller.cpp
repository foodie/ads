#include "ads_controller.h"

#include "log.h"

#include "bidding/ads_bidding_controller.h"
#include "monitor/ads_monitor_controller.h"

// static //
unordered_map<string, AdsController*>* AdsController::map = new unordered_map<string, AdsController*>();
bool AdsController::init()
{
	if ( map == NULL ) {
		WARN("Ads Controller map is null");
		return false;
	}

	// bidding
	AdsController* bc = new AdsBiddingController;
	map->emplace("bidding", bc);

	// monitor
	AdsController* mc = new AdsMonitorController;
	map->emplace("monitor", mc);

	return true;
}

void AdsController::close()
{
	if ( map != NULL ) {
		map->clear();
		map = NULL;
	}
}

AdsController* AdsController::getController(const string &name)
{
	if ( map == NULL ) {
		WARN("Ads Controller doesn't init");
		return NULL;
	}

	auto itr = map->find(name);
	if ( itr == map->end() ) {
		return NULL;
	}

	return itr->second;
}