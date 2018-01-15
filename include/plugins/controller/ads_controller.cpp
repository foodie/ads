#include "plugins/controller/ads_controller.h"

#include "plugins/controller/ads_controller_factory.h"

AdsController* getController(const string &name)
{
	AdsControllerFactory *factory = AdsControllerFactory::getInstance();
    return factory->getController(name);
}