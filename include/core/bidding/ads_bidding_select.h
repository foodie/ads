#ifndef _ADS_BIDDING_SELECT_H
#define _ADS_BIDDING_SELECT_H

#include <list>
#include "core/advertise/ads_advertise_types.h"

using std::list;

class AdsBiddingSelector
{
public:
	AdsAdvertise* select(list<AdsAdvertise*>& ad_list)
	{
		int sel = rand() % ad_list.size();
		for ( auto itr = ad_list.begin(); itr != ad_list.end(); itr++ ) {
			if ( sel == 0 ) {
				return *itr;
			}
			sel--;
		}
		return *( ad_list.begin() );
	}
};

static inline AdsAdvertise* ads_bidding_select(list<AdsAdvertise*>& ad_list)
{
	AdsBiddingSelector selector;
	return selector.select();
}

#endif
/* vim: set ts=4 sw=4 noet: */
