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

typedef bool (*AdsBiddingFilterFunc)(AdsAdvertise*, AdsBiddingParam&);

class AdsBiddingFilterResult
{
public:

private:
	unordered_map<string, vector<int>> result;
};

class AdsBiddingFilter
{
public:
	void filter(AdsBiddingParam& param, list<AdsAdvertise*>& list);

	list<AdsAdvertise*>* getPassAdvertises();

private:
	AdsBiddingFilterResult res;

};

#endif
/* vim: set ts=4 sw=4 noet: */
