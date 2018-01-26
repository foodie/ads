#include "plugins/exchange/ads_exchange.h"

#include <sstream>
#include "ads_conf.h"

using std::ostringstream;

string AdsExchange::getUrlBaseInfo(AdsBiddingParam& param, AdsAdvertise *ad)
{
	ostringstream oss;
	oss << ADS_MONITOR_KEY_BIDDING_ID << "=" << param.impression().id();
	// campaign_id
	oss << "&" << ADS_MONITOR_KEY_CAMPAIGN_ID << "=" << ad->campaign->id;
	// launch_id
	oss << "&" << ADS_MONITOR_KEY_LAUNCH_ID << "=" << ad->launch->id;
	// creative_id
	oss << "&" << ADS_MONITOR_KEY_CREATIVE_ID << "=" << ad->id;
	// material_id
	oss << "&" << ADS_MONITOR_KEY_MATERIAL_ID << "=" << ad->material->id();
	// exchange
	oss << "&" << ADS_MONITOR_KEY_EXCHANGE << "=" << param.exchangeId();
	// bidding_type (RTB | PDB)
	oss << "&" << ADS_MONITOR_KEY_BIDDING_TYPE << "=" << (int)ad->launch->bidding_type;
	// settlement (CPM | CPC)
	oss << "&" << ADS_MONITOR_KEY_SETTLEMENT << "=" << (int)ad->launch->settlement;
	// ad_type
	oss << "&" << ADS_MONITOR_KEY_AD_TYPE << "=" << (int)param.impression().type();
	// os
	oss << "&" << ADS_MONITOR_KEY_OS << "=" << (int)param.device().os();
	// device
	oss << "&" << ADS_MONITOR_KEY_DEVICE_TYPE << "=" << (int)param.device().type();
	// carrier
	oss << "&" << ADS_MONITOR_KEY_CARRIER << "=" << (int)param.device().carrier();
	// connection type
	oss << "&" << ADS_MONITOR_KEY_CONNECTION_TYPE << "=" << (int)param.device().connectionType();
	// ua
	oss << "&" << ADS_MONITOR_KEY_USER_AGENT << "=" << "";
	// idfa
	oss << "&" << ADS_MONITOR_KEY_IDFA << "=" << param.device().idfa();
	// imei
	oss << "&" << ADS_MONITOR_KEY_IMEI << "=" << param.device().imei();
	// ip
	oss << "&" << ADS_MONITOR_KEY_IP << "=" << param.device().ip();
	// address id
	oss << "&" << ADS_MONITOR_KEY_ADDRESS_ID << "=" << param.device().addressId();
	// app name
	oss << "&" << ADS_MONITOR_KEY_APP_NAME << "=" << ads_url_encode(param.app().name());

	return oss.str();
}

string AdsExchange::getWinnoticeUrl(AdsBiddingParam& param, AdsAdvertise *ad)
{
	ostringstream oss;
	oss << g_conf->ads.ads_host << "/monitor/win?" << this->getUrlBaseInfo(param, ad);
	return oss.str();
}

string AdsExchange::getImpressionUrl(AdsBiddingParam& param, AdsAdvertise *ad)
{
	ostringstream oss;
	oss << g_conf->ads.ads_host << "/monitor/imp?" << this->getUrlBaseInfo(param, ad);
	return oss.str();
}

string AdsExchange::getClickUrl(AdsBiddingParam& param, AdsAdvertise *ad)
{
	ostringstream oss;
	oss << g_conf->ads.ads_host << "/monitor/clk?" << this->getUrlBaseInfo(param, ad);
	oss << "&" << ADS_MONITOR_KEY_REDIRECT << "=" << ads_url_encode(ad->landing);
	return oss.str();
}
