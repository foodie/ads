#ifndef _ADS_BIDDING_SERVICE_H
#define _ADS_BIDDING_SERVICE_H

/**
 * 竞价模块对外服务接口类
 */
#include <list>
#include "core/ads_singleton.h"
#include "core/advertise/ads_advertise_types.h"
#include "core/bidding/ads_bidding_param.h"

using std::list;

class AdsBiddingService : public AdsSingleton<AdsBiddingService>
{
	friend class AdsSingleton<AdsBiddingService>;
private:
	AdsBiddingService();

public:
	~AdsBiddingService();

	bool init();
	AdsAdvertise* bidding(const AdsBiddingParam& param,
		list<AdsAdvertise*>& ad_list);
};

inline static AdsBiddingService& getBiddingService()
{
	return AdsBiddingService::getInstance();
}

#endif
/* vim: set ts=4 sw=4 noet: */
