#ifndef _ADS_BIDDING_FILTER_H
#define _ADS_BIDDING_FILTER_H

#include <unordered_map>

using std::unordered_map;

typedef bool (*AdsBiddingFilterFunc) (AdsAdvertise*, AdsBiddingParam&);

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
