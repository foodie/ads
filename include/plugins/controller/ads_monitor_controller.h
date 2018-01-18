#ifndef _ADS_MONITOR_CONTROLLER_H
#define _ADS_MONITOR_CONTROLLER_H

#include "plugins/controller/ads_controller.h"
#include "monitor/ads_monitor_param.h"
#include "monitor/ads_monitor_service.h"

class AdsMonitorController : public AdsController
{
public:

	virtual int process(AdsThreadData* pdata) override;

private:
	bool parseMonitorParam(AdsThreadData* pdata, AdsMonitorParam& param);
	string getDeviceId(AdsMonitorParam& param);

};

#endif
/* vim: set ts=4 sw=4 noet: */
