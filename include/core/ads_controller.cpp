#include "ads_controller.h"

#include "log.h"

#include "bidding/ads_bidding_controller.h"
#include "monitor/ads_monitor_controller.h"

// static //
bool AdsController::init()
{
	if ( map != nullptr ) {
		return true;
	}

	map = new unordered_map<string, AdsController*>();
	if ( map == nullptr ) {
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
	if ( map != nullptr ) {
		map->clear();
		map = nullptr;
	}
}

AdsController* AdsController::getController(const string &name)
{
	if ( map == nullptr ) {
		WARN("Ads Controller doesn't init");
		return nullptr;
	}

	auto itr = map->find(name);
	if ( itr == map->end() ) {
		return nullptr;
	}

	return itr->second;
}