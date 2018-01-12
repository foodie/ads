#ifndef _ADS_ADVIEW_H
#define _ADS_ADVIEW_H

#include "plugins/exchange/ads_exchange.h"

class AdsAdviewExchange : public AdsExchange
{
public:

	virtual bool parseBiddingRequest(AdsHttpRequest *request, 
		AdsBiddingParam& param) override;

	virtual void packBiddingResponse(AdsBiddingParam& param, 
		AdsAdvertise *ad, AdsHttpResponse *response) override;

	virtual int decryptWinPrice(const string& str) override;
};

#endif
/* vim: set ts=4 sw=4 noet: */
