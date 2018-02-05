#include "core/bidding/ads_bidding_filter.h"

#include "core/monitor/ads_monitor_service.h"

static int filter_helper(const AdsBiddingParam& param, list<AdsAdvertise*>& list, 
	AdsBiddingFilterFunc func);


// 设备数据
static bool filter_area_func(const AdsBiddingParam& param, AdsAdvertise* ad);
static bool filter_os_func(const AdsBiddingParam& param, AdsAdvertise* ad);
static bool filter_device_func(const AdsBiddingParam& param, AdsAdvertise* ad);
static bool filter_connectiontype_func(const AdsBiddingParam& param, AdsAdvertise* ad);
static bool filter_carrier_func(const AdsBiddingParam& param, AdsAdvertise* ad);

AdsBiddingFilterFunc filter_funcs[] = {
	filter_area_func,
	filter_os_func,
	filter_device_func,
	filter_connectiontype_func,
	filter_carrier_func,
};


void AdsBiddingFilter::filter(const AdsBiddingParam& param, 
	list<AdsAdvertise*>& list)
{
	for ( AdsBiddingFilterFunc func : filter_funcs ) {
		if ( filter_helper(param, list, func) == 0 ) {
			return ;
		}
	}
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
