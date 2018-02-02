#ifndef _ADS_ADVIEW_EXCHANGE_H
#define _ADS_ADVIEW_EXCHANGE_H

#include "plugins/exchange/ads_exchange.h"
#include "plugins/exchange/ads_adview_param.h"


class AdsAdviewExchange : public AdsExchange
{
public:
	AdsAdviewExchange();
	~AdsAdviewExchange();

	virtual bool parseBiddingRequest(AdsHttpRequest *request, 
		AdsBiddingParam& param) override;

	virtual void packBiddingResponse(AdsBiddingParam& param, 
		AdsAdvertise *ad, AdsHttpResponse *response) override;

	virtual int decryptWinPrice(const string& str) override;

protected:
	virtual string getWinnoticeUrl(AdsBiddingParam& param, 
		AdsAdvertise *ad) override;

	virtual string getImpressionUrl(AdsBiddingParam& param, 
		AdsAdvertise *ad) override;

private:
	void packBiddingFailure(AdsBiddingParam& param, 
		AdsHttpResponse *response);

	void packBiddingSuccess(AdsBiddingParam& param, 
		AdsAdvertise *ad, AdsHttpResponse *response);

/* new */
public:
	virtual bool parseBiddingRequest2(AdsHttpRequest *request, 
		AdsBiddingParam& param, void *buf) override;

	virtual void packBiddingResponse2(AdsBiddingParam& param, void *buf,
		AdsAdvertise *ad, AdsHttpResponse *response) override;

	virtual void biddingFilter2(void *buf, list<AdsAdvertise*>& al) override;
};


#endif
/* vim: set ts=4 sw=4 noet: */
