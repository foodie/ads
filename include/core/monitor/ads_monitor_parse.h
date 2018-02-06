#ifndef _ADS_MONITOR_PARSE_H
#define _ADS_MONITOR_PARSE_H

/**
 * 基本监测参数
 */
#include <string>
#include "core/ads_core.h"
#include "core/ads_request.h"
#include "core/monitor/ads_monitor_param.h"
#include "plugins/exchange/ads_exchange.h"
#include "utils/ads_string.h"
#include "utils/ads_ip_location.h"

using std::string;

class AdsMonitorParse
{
public:
	static bool parseMonitorRequest(AdsHttpRequest *request, AdsMonitorParam *param)
	{
		// type
		string mtype = request->getUri(1);
		if ( mtype == "win" ) {
			param->type = AdsMonitorType::WINNOTICE;
		} else if ( mtype == "imp" ) {
			param->type = AdsMonitorType::IMPRESSION;
		} else if ( mtype == "clk" ) {
			param->type = AdsMonitorType::CLICK;
		} else {
			WARN("[Monitor] monitor type error");
			return false;
		}

		// bidding_id
		param->bidding_id = request->getQuery(ADS_MONITOR_KEY_BIDDING_ID);

		// campaign_id
		param->campaign_id = ads_string_to_int( request->getQuery(ADS_MONITOR_KEY_CAMPAIGN_ID) );
		// launch_id
		param->launch_id = ads_string_to_int( request->getQuery(ADS_MONITOR_KEY_LAUNCH_ID) );
		// creative_id
		param->creative_id = ads_string_to_int( request->getQuery(ADS_MONITOR_KEY_CREATIVE_ID) );
		// material_id
		param->material_id = ads_string_to_int( request->getQuery(ADS_MONITOR_KEY_MATERIAL_ID) );
		
		// exchange
		param->exchange = request->getQuery(ADS_MONITOR_KEY_EXCHANGE);
		// price
		AdsExchange* exchange = getExchange( param->exchange );
		if ( exchange != NULL ) {
			string price = request->getQuery(ADS_MONITOR_KEY_PRICE);
			param->price = exchange->decryptWinPrice(price);
		}
		
		// bidding_type
		string bidding_type = request->getQuery(ADS_MONITOR_KEY_BIDDING_TYPE);
		param->bidding_type = AdsBiddingType( ads_string_to_int(bidding_type) );
		// settlement
		string settlement = request->getQuery(ADS_MONITOR_KEY_SETTLEMENT);
		param->settlement = AdsBiddingSettlement( ads_string_to_int(bidding_type) );
		// ad_type
		string ad_type = request->getQuery(ADS_MONITOR_KEY_AD_TYPE);
		param->ad_type = AdsAdvertiseType( ads_string_to_int(ad_type) );
		
		// os
		string os = request->getQuery(ADS_MONITOR_KEY_OS);
		param->os = AdsOs( ads_string_to_int(os) );
		// device_type
		string device_type = request->getQuery(ADS_MONITOR_KEY_DEVICE_TYPE);
		param->device_type = AdsDeviceType( ads_string_to_int(device_type) );
		// carrier
		string carrier = request->getQuery(ADS_MONITOR_KEY_CARRIER);
		param->carrier = AdsCarrier( ads_string_to_int(carrier) );
		// connection_type
		string connection_type = request->getQuery(ADS_MONITOR_KEY_CONNECTION_TYPE);
		param->connection_type = AdsConnectionType( ads_string_to_int(connection_type) );

		// ua
		param->ua = request->getQuery(ADS_MONITOR_KEY_USER_AGENT);
		// idfa
		param->idfa = request->getQuery(ADS_MONITOR_KEY_IDFA);
		// imei
		param->imei = request->getQuery(ADS_MONITOR_KEY_IMEI);

		// ip
		//long ip = ads_string_to_long( request->getQuery(ADS_MONITOR_KEY_IP) );
		//param->ip = AdsIpLocation::long2Ip(ip);
		param->ip = request->getQuery(ADS_MONITOR_KEY_IP);
		// address_id
		param->address_id = ads_string_to_int( request->getQuery(ADS_MONITOR_KEY_ADDRESS_ID) );

		// appname
		param->appname = request->getQuery(ADS_MONITOR_KEY_APP_NAME);

		return true;
	}
};

#endif
/* vim: set ts=4 sw=4 noet: */
