#ifndef _ADS_BIDDING_CONTROLLER_H
#define _ADS_BIDDING_CONTROLLER_H

#include "ads_controller.h"


class AdsBiddingController : public AdsController
{
public:
	AdsBiddingController() {}

	virtual ~AdsBiddingController() {}


	virtual int perform(AdsThreadData*) override
	{
		return 0;
	}

};

#endif
/* vim: set ts=4 sw=4 noet: */
