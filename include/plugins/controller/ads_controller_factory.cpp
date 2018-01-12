#include "plugins/controller/ads_controller_factory.h"

#include "log.h"
#include "utils/ads_string.h"
#include "plugins/controller/ads_bidding_controller.h"
#include "plugins/controller/ads_monitor_controller.h"

AdsControllerFactory* AdsControllerFactory::instance = new AdsControllerFactory;

AdsControllerFactory::AdsControllerFactory()
{
	controllers = new unordered_map<string, AdsController*>;
	if ( controllers == NULL ) {
		WARN("Controllers is null");
		return false;
	}

	// 添加controller实例
	controllers->emplace("bidding", new AdsBiddingController);
	controllers->emplace("monitor", new AdsMonitorController);
}

AdsControllerFactory::~AdsControllerFactory()
{
	if ( exchanges != NULL ) {
		for ( auto itr = controllers->begin(); itr != controllers->end();
			itr = controllers->erase(itr) ) {
			delete itr->second;
			itr->second = NULL;
		}
	}
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