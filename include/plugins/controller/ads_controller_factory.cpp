#include "plugins/controller/ads_controller_factory.h"

#include "log.h"
#include "utils/ads_string.h"
#include "plugins/controller/ads_bidding_controller.h"
#include "plugins/controller/ads_monitor_controller.h"
#include "plugins/controller/ads_api_controller.h"

AdsControllerFactory::AdsControllerFactory()
{
}

AdsControllerFactory::~AdsControllerFactory()
{
	if ( controllers != NULL ) {
		for ( auto itr = controllers->begin(); itr != controllers->end();
			itr = controllers->erase(itr) ) {
			delete itr->second;
			itr->second = NULL;
		}
	}
}

bool AdsControllerFactory::init()
{
	controllers = new (std::nothrow) unordered_map<string, AdsController*>;
	if ( controllers == NULL ) {
		WARN("Controllers is null");
		return false;
	}

	// 添加controller实例
	controllers->emplace("bidding", new AdsBiddingController);
	controllers->emplace("monitor", new AdsMonitorController);
	controllers->emplace("api", new AdsApiController);

	INFO("[Controller] Controller plugin init success");
	return true;
}

AdsController* AdsControllerFactory::getController(const string& name) const
{
	if ( controllers != NULL ) {
		auto itr = controllers->find( ads_string_tolower(name) );
		if ( itr != controllers->end() ) {
			return itr->second;
		}
	}
	return NULL;
}