#include "plugins/controller/ads_monitor_controller.h"

int AdsMonitorController::process(AdsThreadData* p_thd_data)
{
	bool ret;
	AdsMonitorParam param;

	// 解析
	ret = parseMonitorParam(p_thd_data, param);
	if ( ret ) {
		return -1;
	}

	string dev_id = getDeviceId(param);


	return 0;
}

bool AdsMonitorController::parseMonitorParam(AdsThreadData* p_thd_data, 
	AdsMonitorParam& param)
{

	return true;
}

string AdsMonitorController::getDeviceId(AdsMonitorParam& param)
{

	return "";
}