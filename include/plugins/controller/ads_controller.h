#ifndef _ADS_CONTROLLER_H
#define _ADS_CONTROLLER_H

#include "ads_thread.h"
#include "ads_http.h"
#include "plugins/controller/ads_controller_factory.h"

class AdsController
{
public:
	virtual int process(AdsThreadData*) = 0;
};

inline static AdsController* getController(const string &name)
{
	return AdsControllerFactory::getInstance().getController(name);
}


#endif
/* vim: set ts=4 sw=4 noet: */
