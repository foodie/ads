#ifndef _ADS_EXCHANGE_H
#define _ADS_EXCHANGE_H

#include "ads_request.h"
#include "ads_advertise.h"
#include "bidding/ads_bidding_param.h"

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

#endif
/* vim: set ts=4 sw=4 noet: */
