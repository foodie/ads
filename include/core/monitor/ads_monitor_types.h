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

/**
 * 广告数据监测
 */

class AdsMonitorAdvertise
{
public:
	typedef vector<long> _TimeRecord;
	typedef phashmap<string, _TimeRecord> _UserRecord;

private:
	time_t ts;

	unsigned int cost; // 消耗成本
	unsigned int imp;	// 展示数
	unsigned int clk;	// 点击数

	_UserRecord records;
};

// 活动监测
class AdsMonitorCampaign
{
public:
	AdsMonitorCampaign() 
		: ts(0), cost(0), imp(0), clk(0), records(MAX_MONITOR_BITEMS_SIZE)
	{}

private:
	time_t ts;

	unsigned int cost; // 消耗成本
	unsigned int imp;	// 展示数
	unsigned int clk;	// 点击数

	AdsMonitorUserRecord records;
};

// 投放监测
class AdsMonitorLaunch
{
public:
	AdsMonitorLaunch(AdsMonitorCampaign *campaign) 
		: ts(0), cost(0), imp(0), clk(0), records(MAX_MONITOR_BITEMS_SIZE)
	{}

	unsigned int getCost() { return cost; }
	unsigned int getImp() { return imp; }
	unsigned int getClk() { return clk; }

private:
	time_t ts;

	unsigned int cost; 	// 当天消耗成本
	unsigned int imp;	// 展示数
	unsigned int clk;	// 点击数
	
	AdsMonitorUserRecord records;
};

// 创意监测
class AdsMonitorCreative
{
};



#endif
/* vim: set ts=4 sw=4 noet: */
