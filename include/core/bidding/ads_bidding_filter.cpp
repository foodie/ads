#include "core/bidding/ads_bidding_filter.h"

#include "core/monitor/ads_monitor_service.h"

static int filter_helper(const AdsBiddingParam& param, list<AdsAdvertise*>& list, 
	AdsBiddingFilterFunc func);

// 媒体
static bool filter_exchange_func(const AdsBiddingParam& param, AdsAdvertise* ad);
// pmp
static bool filter_pmp_func(const AdsBiddingParam& param, AdsAdvertise* ad);
// 出价
static bool filter_price_func(const AdsBiddingParam& param, AdsAdvertise* ad);
// 定向数据
static bool filter_area_func(const AdsBiddingParam& param, AdsAdvertise* ad);
static bool filter_os_func(const AdsBiddingParam& param, AdsAdvertise* ad);
static bool filter_device_func(const AdsBiddingParam& param, AdsAdvertise* ad);
static bool filter_connectiontype_func(const AdsBiddingParam& param, AdsAdvertise* ad);
static bool filter_carrier_func(const AdsBiddingParam& param, AdsAdvertise* ad);
// 广告位
static bool filter_zone_func(const AdsBiddingParam& param, AdsAdvertise* ad);
// 素材


AdsBiddingFilterFunc[] rtb_funcs = {
	filter_price_func,
	filter_area_func,
	filter_os_func,
	filter_device_func,
	filter_connectiontype_func,
	filter_carrier_func,
};

AdsBiddingFilterFunc[] pdb_funcs = {
	filter_pmp_func,
};

void AdsBiddingFilter::filter(const AdsBiddingParam& param, 
	list<AdsAdvertise*>& list)
{
	// 过滤媒体
	if ( filter_helper(param, list, filter_exchange_func) == 0 ) {
		return ;
	}

	AdsBiddingType btype = param.impression().biddingType();
	if ( btype == AdsBiddingType::RTB ) {
		rtb_filter(param, list);
	} else if ( btype == AdsBiddingType::PDB ) {
		pdb_filter(param, list);
	}

	user_filter(param, list);
}

// rtb
void AdsBiddingFilter::rtb_filter(const AdsBiddingParam& param, 
	list<AdsAdvertise*>& list)
{
	for ( AdsBiddingFilterFunc func : rtb_funcs ) {
		if ( filter_helper(param, list, func) == 0 ) {
			return ;
		}
	}
}

// pdb
void AdsBiddingFilter::pdb_filter(const AdsBiddingParam& param, 
	list<AdsAdvertise*>& list)
{
	for ( AdsBiddingFilterFunc func : pdb_funcs ) {
		if ( filter_helper(param, list, func) == 0 ) {
			return ;
		}
	}
}

// freq	
void AdsBiddingFilter::user_filter(const AdsBiddingParam& param, 
	list<AdsAdvertise*>& list)
{
	// 对用户频次进行过滤


}


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

/**
 * cmp func
 */
static bool filter_exchange_func(const AdsBiddingParam& param, AdsAdvertise* ad)
{
	AdsLaunch *launch = ad->launch;
	if ( launch == NULL ) {
		return false;
	}
	if ( param.exchangeId() != launch->exchangeid ) {
		return false;
	}
	return true;
}

static bool filter_pmp_func(const AdsBiddingParam& param, AdsAdvertise* ad)
{
	auto pmp = param.impression().pmp();
	AdsLaunch *launch = ad->launch;
	if ( launch == NULL ) {
		return false;
	}
	if ( pmp.id() != launch->pmp_id ) {
		return false;
	}
	return true;
}

static bool filter_price_func(const AdsBiddingParam& param, AdsAdvertise* ad)
{
	auto imp = param.impression();
	AdsLaunch *launch = ad->launch;
	if ( launch == NULL ) {
		return false;
	}
	if ( imp.bidFloor() > launch->price ) {
		return false;
	}
	return true;
}

static bool filter_area_func(const AdsBiddingParam& param, AdsAdvertise* ad)
{
	auto device = param.device();
	AdsLaunch *launch = ad->launch;
	if ( launch == NULL ) {
		return false;
	}
	if ( launch->area.size() > 0 ) {
		auto got = launch->area.find( device.addressId() );
		if ( got == launch->area.end() ) {
			return false;
		}
	}
	return true;
}

static bool filter_os_func(const AdsBiddingParam& param, AdsAdvertise* ad)
{
	auto device = param.device();
	AdsLaunch *launch = ad->launch;
	if ( launch == NULL ) {
		return false;
	}
	AdsOsOrientation org = launch->os;
	AdsOs os = device.os();
	if ( org == AdsOsOrientation::IOS && os != AdsOs::IOS ) {
		return false;
	}
	if ( org == AdsOsOrientation::ANDROID && os != AdsOs::ANDROID ) {
		return false;
	}
	return true;
}

static bool filter_device_func(const AdsBiddingParam& param, AdsAdvertise* ad)
{
	auto device = param.device();
	AdsLaunch *launch = ad->launch;
	if ( launch == NULL ) {
		return false;
	}
	AdsDeviceOrientation org = launch->device;
	AdsDeviceType type = device.type();
	if ( org == AdsDeviceOrientation::MOBILE 
		&& type != AdsDeviceType::MOBILE ) {
		return false;
	}
	if ( org == AdsDeviceOrientation::PAD 
		&& type != AdsDeviceType::PAD ) {
		return false;
	}
	return true;
}

static bool filter_connectiontype_func(const AdsBiddingParam& param, AdsAdvertise* ad)
{
	auto device = param.device();
	AdsLaunch *launch = ad->launch;
	if ( launch == NULL ) {
		return false;
	}
	if ( launch->connection_type.size() > 0 ) {
		auto got = launch->connection_type.find( (int)device.connectionType() );
		if ( got == launch->connection_type.end() ) {
			return false;
		}
	}
	return true;
}
static bool filter_carrier_func(const AdsBiddingParam& param, AdsAdvertise* ad)
{
	auto device = param.device();
	AdsLaunch *launch = ad->launch;
	if ( launch == NULL ) {
		return false;
	}
	if ( launch->carrier.size() > 0 ) {
		auto got = launch->carrier.find( (int)device.carrier() );
		if ( got == launch->carrier.end() ) {
			return false;
		}
	}
	return true;
}
static bool filter_zone_func(const AdsBiddingParam& param, AdsAdvertise* ad)
{
	auto imp = param.impression();
	if ( imp.zoneId() != ad->zone_id ) {
		return false;
	}
	return true;
}
