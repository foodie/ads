#ifndef _ADS_EXCHANGE_H
#define _ADS_EXCHANGE_H

#include "ads_request.h"
#include "core/advertise/ads_advertise_types.h"
#include "core/bidding/ads_bidding_param.h"
#include "plugins/exchange/ads_exchange_factory.h"

class AdsExchange
{
public:
	AdsExchange() {}
	virtual ~AdsExchange() {}

	virtual bool parseBiddingRequest(AdsHttpRequest *request, 
		AdsBiddingParam& param, void *buf) = 0;

	virtual void packBiddingResponse(AdsBiddingParam& param, void *buf,
		AdsAdvertise *ad, AdsHttpResponse *response) = 0;

	virtual int decryptWinPrice(const string& str)
	{
		return atoi( str.c_str() );
	}

	virtual void biddingFilter(void *buf, list<AdsAdvertise*>& al) = 0;

protected:

	virtual string getUrlBaseInfo(AdsBiddingParam& param, AdsAdvertise *ad);

	virtual string getWinnoticeUrl(AdsBiddingParam& param, AdsAdvertise *ad);

	virtual string getImpressionUrl(AdsBiddingParam& param, AdsAdvertise *ad);

	virtual string getClickUrl(AdsBiddingParam& param, AdsAdvertise *ad);

	virtual string getMaterialSourceUrl(AdsAdvertise *ad);
};

inline static bool initExchange()
{
	return AdsExchangeFactory::getInstance().init();
}

inline static AdsExchange* getExchange(const string& name)
{
	return AdsExchangeFactory::getInstance().getExchange(name);
}

#endif
/* vim: set ts=4 sw=4 noet: */
