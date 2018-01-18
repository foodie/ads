#include "core/bidding/ads_bidding_filter.h"

/*
static int filter_helper(const AdsBiddingParam& param, list<AdsAdvertise*>& list, 
	AdsBiddingFilterFunc func);

static void bool filter_exchange_func(const AdsBiddingParam& param, 
	const AdsAdvertise* ad);
*/

void AdsBiddingFilter::filter(const AdsBiddingParam& param, 
	list<AdsAdvertise*>& list)
{
	// 过滤媒体
/*
	if ( filter_helper(param, list, filter_exchange_func) == 0 ) {
		return ;
	}
*/

	AdsBiddingType btype = param.impression().biddingType();
	if ( btype == AdsBiddingType::RTB ) {
		rtb_filter(param, list);
	} else if ( btype == AdsBiddingType::PDB ) {
		pdb_filter(param, list);
	}
}


void AdsBiddingFilter::rtb_filter(const AdsBiddingParam& param, 
	list<AdsAdvertise*>& list)
{

}

void AdsBiddingFilter::pdb_filter(const AdsBiddingParam& param, 
	list<AdsAdvertise*>& list)
{

}
	
void AdsBiddingFilter::user_filter(const AdsBiddingParam& param, 
	list<AdsAdvertise*>& list)
{

}

/*
static int filter_helper(const AdsBiddingParam& param, list<AdsAdvertise*>& list, 
	AdsBiddingFilterFunc func)
{
	for (auto itr = list.begin(); itr != list.end(); ) {
		if ( func(param, *itr) ) {
			itr++;
		} else {
			itr = list.erase(itr);
		}
	}
	return list.size();
}

static void bool filter_exchange_func(const AdsBiddingParam& param, 
	const AdsAdvertise* ad)
{
	return false;
}
*/