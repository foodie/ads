#ifndef _ADS_BIDDING_EVALUATE_H
#define _ADS_BIDDING_EVALUATE_H

#include <list>
#include "core/advertise/ads_advertise_types.h"
#include "core/bidding/ads_bidding_param.h"

class AdsBiddingEvaluateValue
{
public:
	int value;
	AdsAdvertise *ad;
};

class AdsBiddingEvaluate
{
public:
	void evaluate(const AdsBiddingParam& param, list<AdsAdvertise*>& list)
	{

	}
};

static inline void ads_bidding_evaluate(const AdsBiddingParam& param, 
	list<AdsAdvertise*>& list)
{
	AdsBiddingEvaluate evaluate;
	evaluate.evaluate(param, list);
}

#endif
/* vim: set ts=4 sw=4 noet: */
