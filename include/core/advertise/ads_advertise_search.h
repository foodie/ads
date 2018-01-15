#ifndef _ADS_ADVERTISE_SEARCH_H
#define _ADS_ADVERTISE_SEARCH_H

#include <list>
#include "core/advertise/ads_advertise_types.h"

class AdsAdvertiseSearch
{
public:
	static void search(AdsBiddingParam& param, list<AdsAdvertise*>& list);

};

#endif
/* vim: set ts=4 sw=4 noet: */
