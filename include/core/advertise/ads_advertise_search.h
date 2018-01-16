#ifndef _ADS_ADVERTISE_SEARCH_H
#define _ADS_ADVERTISE_SEARCH_H

#include <list>
#include "core/advertise/ads_advertise_types.h"
#include "core/advertise/ads_advertise_manager.h"

#include "utils/ads_time.h"

using std::list;

class AdsAdvertiseSearch
{
public:
	static void search(list<AdsAdvertise*>& list)
	{
		AdsAdvertiseCollection *collection;

		// 当前时间戳
		time_t nowtime = ads_nowtime();

		// 获取所有活动列表
		list<AdsCampaign*> campaigns;
		collection->getAllCampaigns(campaigns);

		for ( AdsCampaign* campaign : campaigns ) {

			// 过滤
			if ( campaign->start_date > nowtime 
				|| campaign->end_date < nowtime ) {
				continue;
			}

			for ( AdsLaunch* launch : campaign->launchs ) {
				// 过滤
				if ( launch->start_date > nowtime
					|| launch->end_date < nowtime ) {
					continue;
				}

				for ( AdsCreative* creative : launch->creatives ) {
					// 过滤
					if (creative->effective == 1) {
						if ( creative->start_date > nowtime 
							|| creative->end_date < nowtime ) {
							continue;
						}
					}
					list.push_back(creative);
				}
			}
		}
	}

	static AdsAdvertise* search(id)
	{
		
	}

};

#endif
/* vim: set ts=4 sw=4 noet: */
