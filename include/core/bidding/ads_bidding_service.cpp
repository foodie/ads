#include "core/bidding/ads_bidding_service.h"

#include <list>
#include "core/bidding/ads_bidding_param.h"
#include "core/advertise/ads_advertise_service.h"

using std::list;

AdsBiddingService::AdsBiddingService()
{
}

AdsBiddingService::~AdsBiddingService()
{

}

bool AdsBiddingService::init()
{

	return true;
}

AdsAdvertise* AdsBiddingService::bidding(const AdsBiddingParam* param)
{
	list<AdsAdvertise*> ad_list;
	string dev_id = getDeviceId(param.device()); // 设备id

	AdsAdvertiseService& ad_serv = getAdvertiseService();
	ad_serv.search(dev_id, ad_list);



	return NULL;
}