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
		AdsBiddingParam& param, void *buf) override;

	virtual void packBiddingResponse(AdsBiddingParam& param, void *buf,
		AdsAdvertise *ad, AdsHttpResponse *response) override;

	virtual void biddingFilter(void *buf, list<AdsAdvertise*>& al) override;

	virtual int decryptWinPrice(const string& str) override;

protected:
	virtual string getWinnoticeUrl(AdsBiddingParam& param, 
		AdsAdvertise *ad) override;

	virtual string getImpressionUrl(AdsBiddingParam& param, 
		AdsAdvertise *ad) override;

private:
	void packBiddingFailure(AdsBiddingParam& param, AdsHttpResponse *response);

	void packBiddingSuccess(AdsBiddingParam& param, AdsAdviewBidRequest *req, 
		AdsAdvertise *ad, AdsHttpResponse *response);
};


#endif
/* vim: set ts=4 sw=4 noet: */
