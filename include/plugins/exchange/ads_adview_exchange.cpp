#include "plugins/exchange/ads_adview_exchange.h"

bool AdsAdviewExchange::parseBiddingRequest(AdsHttpRequest *request, 
	AdsBiddingParam& param)
{
	return true;
}

void AdsAdviewExchange::packBiddingResponse(AdsBiddingParam& param,
	AdsAdvertise *ad, AdsHttpResponse *response)
{

}

int AdsAdviewExchange::decryptWinPrice(const string& str)
{
	return 0;	
}