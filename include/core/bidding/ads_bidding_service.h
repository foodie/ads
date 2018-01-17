#ifndef _ADS_BIDDING_SERVICE_H
#define _ADS_BIDDING_SERVICE_H

/**
 * 竞价模块对外服务接口类
 */
#include "core/ads_singleton.h"
#include "core/advertise/ads_advertise_types.h"

class AdsBiddingParam;

class AdsBiddingService : public AdsSingleton<AdsBiddingService>
{
	friend class AdsSingleton<AdsBiddingService>;
private:
	AdsBiddingService();

public:
	~AdsBiddingService();

	AdsAdvertise* bidding(const AdsBiddingParam* param);
};

inline static AdsBiddingService& getBiddingService()
{
	return AdsBiddingService::getInstance();
}

#endif
/* vim: set ts=4 sw=4 noet: */
