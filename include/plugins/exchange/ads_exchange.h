#ifndef _ADS_EXCHANGE_H
#define _ADS_EXCHANGE_H

#include "ads_request.h"
#include "core/advertise/ads_advertise_types.h"
#include "core/bidding/ads_bidding_param.h"
#include "plugins/exchange/ads_exchange_factory.h"

class AdsExchange
{
public:

	virtual bool parseBiddingRequest(AdsHttpRequest *request, 
		AdsBiddingParam& param) = 0;

	virtual void packBiddingResponse(AdsBiddingParam& param, 
		AdsAdvertise *ad, AdsHttpResponse *response) = 0;

	virtual int decryptWinPrice(const string& str)
	{
		return atoi( str.c_str() );
	}
};

inline static AdsExchange* getExchange(const string& name)
{
	return AdsExchangeFactory::getInstance().getExchange(name);
}

#endif
/* vim: set ts=4 sw=4 noet: */
