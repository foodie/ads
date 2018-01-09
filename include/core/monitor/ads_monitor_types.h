#ifndef _ADS_MONITOR_COUNTER_H
#define _ADS_MONITOR_COUNTER_H

/**
 * 监测数据的数据结构
 */

#include <sys/time.h>
#include <string>
#include <vector>
#include "phashmap.h"

using std::string;
using std::vector;
using lib::phashmap;

typedef vector<long> AdsMonitorTimeRecord;
typedef phashmap<string, AdsMonitorTimeRecord> AdsMonitorUserRecord;

// 活动监测
class AdsMonitorCampaign
{
public:
	AdsMonitorCampaign() 
		: launchs(MAX_MONITOR_BITEMS_SIZE),
		  cost(0), imp(0), clk(0)
	{}

private:
	time_t ts;

	unsigned int cost; // 消耗成本
	unsigned int imp;	// 展示数
	unsigned int clk;	// 点击数

	phashmap<int, AdsMonitorLaunch*> launchs; // 下属投放
};

// 投放监测
class AdsMonitorLaunch
{
public:
	AdsMonitorLaunch(AdsMonitorCampaign *campaign) 
		: campaign(campaign), creatives(MAX_MONITOR_BITEMS_SIZE), record(MAX_MONITOR_BITEMS_SIZE)
		  cost(0), imp(0), clk(0)
	{}

	unsigned int getCost() { return cost; }
	unsigned int getImp() { return imp; }
	unsigned int getClk() { return clk; }

private:
	unsigned int cost; 	// 当天消耗成本
	unsigned int imp;	// 展示数
	unsigned int clk;	// 点击数
	
	AdsMonitorUserRecord record;

	AdsMonitorCampaign *campaign; // 所属活动
	phashmap<int, AdsMonitorCreative*> creatives; // 下属创意
};

// 创意监测
class AdsMonitorCreative
{
};



#endif
/* vim: set ts=4 sw=4 noet: */
