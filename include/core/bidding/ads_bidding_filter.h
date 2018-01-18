#ifndef _ADS_BIDDING_FILTER_H
#define _ADS_BIDDING_FILTER_H

#include <string>
#include <list>
#include <vector>
#include <unordered_map>
#include "core/advertise/ads_advertise_types.h"
#include "core/bidding/ads_bidding_param.h"

using std::string;
using std::list;
using std::vector;
using std::unordered_map;

typedef bool (*AdsBiddingFilterFunc)(const AdsBiddingParam&, const AdsAdvertise*);


class AdsBiddingFilter
{
public:
	void filter(const AdsBiddingParam& param, list<AdsAdvertise*>& list);

	void rtb_filter(const AdsBiddingParam& param, list<AdsAdvertise*>& list);
	void pdb_filter(const AdsBiddingParam& param, list<AdsAdvertise*>& list);
	
	void user_filter(const AdsBiddingParam& param, list<AdsAdvertise*>& list);
};

static inline void ads_bidding_filter(const AdsBiddingParam& param, 
	list<AdsAdvertise*>& list)
{
	AdsBiddingFilter filter;
	filter.filter(param, list);
}

#endif
/* vim: set ts=4 sw=4 noet: */
