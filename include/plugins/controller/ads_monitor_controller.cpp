#include "monitor/ads_monitor_controller.h"

int AdsMonitorController::perform(AdsThreadData* p_thd_data)
{
	bool ret;
	AdsMonitorParam param;

	// 解析
	ret = parseMonitorParam(pdata, param);
	if ( ret ) {
		AdsMonitorLogger::logWarn();
		return -1;
	}

	string dev_id = getDeviceId(param);

	if ( param.type == AdsMonitorType::WINNOTICE ) {
		// 赢价

	} else if ( param.type == AdsMonitorType::IMPRESSION ) {
		// 展示

		AdsMonitorService::addCampaignImp( param.campaign_id, dev_id );
		AdsMonitorService::addLaunchImp( param.launch_id, dev_id );

	} else if ( param.type == AdsMonitorType::CLICK ) {
		// 点击

		AdsMonitorService::addCampaignClk( param.campaign_id, dev_id );
		AdsMonitorService::addLaunchClk( param.launch_id, dev_id );

	}

	AdsMonitorLogger::logTrack(param);
	return 0;
}

bool AdsMonitorController::parseMonitorParam(AdsThreadData* pdata, AdsMonitorParam& mdata)
{

	return true;
}

string AdsMonitorController::getDeviceId(AdsMonitorParam& param)
{

	return "";
}