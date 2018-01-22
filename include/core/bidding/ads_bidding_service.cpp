#include "core/bidding/ads_bidding_service.h"

#include "log.h"
#include "core/bidding/ads_bidding_filter.h"
#include "core/bidding/ads_bidding_evaluate.h"
#include "core/bidding/ads_bidding_select.h"

AdsBiddingService::AdsBiddingService()
{
}

AdsBiddingService::~AdsBiddingService()
{

}

bool AdsBiddingService::init()
{

	INFO("[Bidding] Bidding Service init success");
	return true;
}

AdsAdvertise* AdsBiddingService::bidding(const AdsBiddingParam& param,
	list<AdsAdvertise*>& ad_list)
{
	// 过滤
	ads_bidding_filter(param, ad_list);
	if ( ad_list.size() == 0 ) {
		return NULL;
	}

	// 评估
	//vector<AdsBiddingEvaluateValue> value( ad_list.size() );
	//ads_bidding_evaluate(param, ad_list, value);

	// 选择
	AdsAdvertise *ad = ads_bidding_select(ad_list);

	return ad;
}