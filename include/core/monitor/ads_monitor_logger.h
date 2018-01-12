#ifndef _ADS_MONITOR_LOGGER_H
#define _ADS_MONITOR_LOGGER_H

/**
 * 打印监测日志
 */

#include "log.h"
#include "core/monitor/ads_monitor_data.h"

class AdsMonitorLogger
{
public:
	static void logTrack(AdsMonitorData& mdata);
	static void logWarn();
};

#endif
/* vim: set ts=4 sw=4 noet: */
