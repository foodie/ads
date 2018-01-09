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
		AdsMonitorParam data;

		// 解析
		ret = parseQuery(pdata, data);
		if ( ret ) {
			AdsMonitorLogger::logWarn();
			return -1;
		}

		AdsMonitorLogger::logTrack(data);
		return 0;
	}

private:
	bool parseQuery(AdsThreadData* pdata, AdsMonitorParam& mdata);

};

#endif
/* vim: set ts=4 sw=4 noet: */
