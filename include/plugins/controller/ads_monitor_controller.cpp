#include "plugins/controller/ads_monitor_controller.h"

#include <iostream>
#include "core/monitor/ads_monitor_parse.h"
#include "core/monitor/ads_monitor_service.h"

#define LOG_MONITOR_PARAM 0

static void log_monitor_param(AdsMonitorParam& param);

int AdsMonitorController::process(AdsThreadData* p_thd_data)
{
	bool ret;
	AdsMonitorParam param;

	ret = AdsMonitorParse::parseMonitorRequest(p_thd_data->request, &param);
	if ( !ret ) {
		WARN("[Monitor] parse request failed");
		return ADS_HTTP_BAD_REQUEST;
	}

	if ( LOG_MONITOR_PARAM ) {
		log_monitor_param(param);
	}

	AdsMonitorService& serv = getMonitorService();
	serv.update(&param);

	return ADS_HTTP_OK;
}

static void log_monitor_param(AdsMonitorParam& param)
{
	using std::cout;
	using std::endl;

	DEBUG("Log Monitor Param");
	cout << "---------- Bidding Param ----------" << endl;

	cout << "type: " 		 	<< (int)param.type << endl;
	cout << "bidding_id: " 		<< param.bidding_id << endl;

	cout << "campaign_id: " 	<< param.campaign_id << endl;
	cout << "launch_id: " 		<< param.launch_id << endl;
	cout << "creative_id: " 	<< param.creative_id << endl;
	cout << "material_id: " 	<< param.material_id << endl;

	cout << "exchange: " 		<< param.exchange << endl;
	cout << "price: " 		 	<< param.price << endl;

	cout << "bidding_type: " 	<< (int)param.bidding_type << endl;
	cout << "settlement: " 		<< (int)param.settlement << endl;
	cout << "ad_type: " 		<< (int)param.ad_type << endl;

	cout << "os: " 		 		<< (int)param.os << endl;
	cout << "device_type: " 	<< (int)param.device_type << endl;
	cout << "carrier: " 		<< (int)param.carrier << endl;
	cout << "connection_type: " << (int)param.connection_type << endl;

	cout << "ua: " 		 		<< param.ua << endl;
	cout << "idfa: " 		 	<< param.idfa << endl;
	cout << "imei: " 		 	<< param.imei << endl;

	cout << "ip: " 		 		<< param.ip << endl;
	cout << "address_id: " 		<< param.address_id << endl;

	cout << "appname: " 		<< param.appname << endl;
	
	cout << "------------------------------------" << endl;
}
