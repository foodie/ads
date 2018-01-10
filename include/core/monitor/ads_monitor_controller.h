#ifndef _ADS_MONITOR_CONTROLLER_H
#define _ADS_MONITOR_CONTROLLER_H

#include "ads_controller.h"
#include "monitor/ads_monitor_param.h"
#include "monitor/ads_monitor_logger.h"

class AdsMonitorController : public AdsController
{
public:
	AdsMonitorController() {}
	virtual ~AdsMonitorController() {}

	virtual int perform(AdsThreadData* pdata) override
	{
		bool ret;
		AdsMonitorParam param;

		// 解析
		ret = getMonitorParamFromQuery(pdata, param);
		if ( ret ) {
			AdsMonitorLogger::logWarn();
			return -1;
		}

		

		AdsMonitorLogger::logTrack(param);
		return 0;
	}

private:
	bool getMonitorParamFromQuery(AdsThreadData* pdata, AdsMonitorParam& param);

};

#endif
/* vim: set ts=4 sw=4 noet: */
