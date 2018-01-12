#include "plugins/controller/ads_controller_factory.h"

#include "log.h"

#include "utils/ads_string.h"

unordered_map<string, AdsController*>* AdsControllerFactory::controllers = new unordered_map<string, AdsController*>;

bool AdsControllerFactory::init()
{

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