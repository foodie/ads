#ifndef _ADS_DEF_H
#define _ADS_DEF_H

#include <serv.h>
#include <signal.h>
#include <gflags/gflags.h>

#include "conf.h"
extern "C" {
#include <ini_config.h>
}

#include "ads_core.h"

// 监测接口
#define ADS_MONITOR_WINNOTICE_API		"monitor/winnotice" 
#define ADS_MONITOR_IMPRESSION_API		"monitor/impression"
#define ADS_MONITOR_CLICK_API			"monitor/click"



#endif
/* vim: set ts=4 sw=4 noet: */
