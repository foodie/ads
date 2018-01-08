#ifndef _ADS_MONITOR_CONTROLLER_H
#define _ADS_MONITOR_CONTROLLER_H

#include "ads_controller.h"


class AdsMonitorController : public AdsController
{
public:
	AdsMonitorController() {}
	virtual ~AdsMonitorController() {}

	virtual int perform(AdsThreadData*) override
	{
		return 0;
	}

};

#endif
/* vim: set ts=4 sw=4 noet: */
