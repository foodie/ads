#ifndef _ADS_CONTROLLER_H
#define _ADS_CONTROLLER_H

#include "ads_thread.h"
#include "ads_http.h"

class AdsController
{
public:
	virtual int process(AdsThreadData*) = 0;
};

AdsController* getController(const string &name);

#endif
/* vim: set ts=4 sw=4 noet: */
