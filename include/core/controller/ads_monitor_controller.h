#ifndef _ADS_MONITOR_CONTROLLER_H
#define _ADS_MONITOR_CONTROLLER_H

#include "ads_controller.h"
#include "monitor/ads_monitor_param.h"
#include "monitor/ads_monitor_service.h"
#include "monitor/ads_monitor_logger.h"

class AdsMonitorController : public AdsController
{
public:
	AdsMonitorController() {}
	virtual ~AdsMonitorController() {}

	virtual int perform(AdsThreadData* pdata) override;

private:
	bool parseMonitorParam(AdsThreadData* pdata, AdsMonitorParam& param);
	string getDeviceId(AdsMonitorParam& param);

};

#endif
/* vim: set ts=4 sw=4 noet: */