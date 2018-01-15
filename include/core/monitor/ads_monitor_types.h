#ifndef _ADS_MONITOR_TYPES_H
#define _ADS_MONITOR_TYPES_H

/**
 * 监测数据的数据结构
 */
#include <string>
#include <vector>
#include "phashmap.h"
#include "utils/ads_time.h"

using std::string;
using std::vector;
using lib::phashmap;

const unsigned int MAX_MONITOR_BITEMS_SIZE = 1000;

enum class AdsMonitorRecordMode { DAY=1, WEEK, MONTH, ALL };

//typedef vector<time_t> AdsMonitorTimeRecord;

class AdsMonitorTimeRecord
{
public:
	void addRecord(time_t ts)
	{
		records.push_back(ts);
	}

	int getFreq(AdsMonitorRecordMode m)
	{
		if ( m == AdsMonitorRecordMode::ALL ) {
			return records.size();
		}

		time_t t;
		if ( m == AdsMonitorRecordMode::DAY ) {
			t = ads_today();
		} else if ( m == AdsMonitorRecordMode::WEEK ) {
			t = ads_first_day_of_week();
		} else if ( m == AdsMonitorRecordMode::MONTH ) {
			t = ads_first_day_of_month();
		}

		int i;
		for ( i = records.size() - 1; i >= 0; i-- ) {
			if ( records[i] < t ) {
				break;
			}
		}

		return records.size() - 1 - i;
	}

private:
	vector<time_t> records;
};

class AdsMonitorUserRecord
{
public:

	void addImpRecord(time_t ts)
	{
		imp.addRecord(ts);
	}

	int getImpFreq(AdsMonitorRecordMode m)
	{
		return imp.getFreq(m);
	}

	void addClkRecord(time_t ts)
	{
		clk.addRecord(ts);
	}

	int getClkFreq(AdsMonitorRecordMode m)
	{
		return clk.getFreq(m);
	}

private:
	AdsMonitorTimeRecord imp; // 曝光记录
	AdsMonitorTimeRecord clk; // 点击记录
};



// 活动监测
class AdsMonitorCampaign
{
public:
	AdsMonitorCampaign() 
		: read_ts(0),write_ts(0), 
		  cost(0),imp(0),clk(0), 
		  records(MAX_MONITOR_BITEMS_SIZE)
	{}

	unsigned int getCost() { return cost; }
	void setCost(unsigned int c) { cost = c; }
	void incCost(unsigned int c) { cost += c; }

	unsigned int getImp() { return imp; }
	void setImp(unsigned int i) { imp = i; }
	void incImp(unsigned int i=1) { imp += i; }

	unsigned int getClk() { return clk; }
	void setClk(unsigned int c) { clk = c; }
	void incClk(unsigned int c=1) { clk += c; }

	void addImpRecord(const string& name, time_t ts) 
	{ 
		AdsMonitorUserRecord *record = getUserRecord(name);
		record->addImpRecord(ts);
	}

	int getImpFreq(const string& name, AdsMonitorRecordMode m)
	{
		AdsMonitorUserRecord *record = findUserRecord(name);
		if ( record == NULL ) {
			return 0;
		}
		return record->getImpFreq(m);
	}

	void addClkRecord(const string& name, time_t ts)
	{
		AdsMonitorUserRecord *record = getUserRecord(name);
		record->addClkRecord(ts);
	}

	int getClkFreq(const string& name, AdsMonitorRecordMode m)
	{
		AdsMonitorUserRecord *record = findUserRecord(name);
		if ( record == NULL ) {
			return 0;
		}
		return record->getClkFreq(m);
	}

private:
	time_t read_ts;
	time_t write_ts;

	// 总数据
	unsigned int cost;  // 消耗成本
	unsigned int imp;	// 展示数
	unsigned int clk;	// 点击数

	phashmap<string, AdsMonitorUserRecord*> records;

	AdsMonitorUserRecord* findUserRecord(const string& name)
	{
		AdsMonitorUserRecord *record;
		if ( records.get(name, &record) == lib::HASH_NOEXIST ) {
			return NULL;
		}
		return record;
	}

	AdsMonitorUserRecord* getUserRecord(const string& name)
	{
		AdsMonitorUserRecord *record = findUserRecord(name);
		if ( record == NULL ) {
			record = new (std::nothrow) AdsMonitorUserRecord;
			records.set(name, record);
		}
		return record;
	}
};

// 投放监测
class AdsMonitorLaunch
{
public:
	AdsMonitorLaunch() 
		: read_ts(0),write_ts(0), 
		  cost(0),imp(0),clk(0), 
		  records(MAX_MONITOR_BITEMS_SIZE)
	{}

	unsigned int getCost() { return cost; }
	void setCost(unsigned int c) { cost = c; }
	void incCost(unsigned int c) { cost += c; }

	unsigned int getImp() { return imp; }
	void setImp(unsigned int i) { imp = i; }
	void incImp(unsigned int i=1) { imp += i; }

	unsigned int getClk() { return clk; }
	void setClk(unsigned int c) { clk = c; }
	void incClk(unsigned int c=1) { clk += c; }

	void addImpRecord(const string& name, time_t ts) 
	{ 
		AdsMonitorUserRecord *record = getUserRecord(name);
		record->addImpRecord(ts);
	}

	int getImpFreq(const string& name, AdsMonitorRecordMode m)
	{
		AdsMonitorUserRecord *record = findUserRecord(name);
		if ( record == NULL ) {
			return 0;
		}
		return record->getImpFreq(m);
	}

	void addClkRecord(const string& name, time_t ts)
	{
		AdsMonitorUserRecord *record = getUserRecord(name);
		record->addClkRecord(ts);
	}

	int getClkFreq(const string& name, AdsMonitorRecordMode m)
	{
		AdsMonitorUserRecord *record = findUserRecord(name);
		if ( record == NULL ) {
			return 0;
		}
		return record->getClkFreq(m);
	}

private:
	time_t read_ts;
	time_t write_ts;

	// 当天数据
	unsigned int cost; 	// 消耗成本
	unsigned int imp;	// 展示数
	unsigned int clk;	// 点击数
	
	phashmap<string, AdsMonitorUserRecord*> records;

	AdsMonitorUserRecord* findUserRecord(const string& name)
	{
		AdsMonitorUserRecord *record;
		if ( records.get(name, &record) == lib::HASH_NOEXIST ) {
			return NULL;
		}
		return record;
	}

	AdsMonitorUserRecord* getUserRecord(const string& name)
	{
		AdsMonitorUserRecord *record = findUserRecord(name);
		if ( record == NULL ) {
			record = new (std::nothrow) AdsMonitorUserRecord;
			records.set(name, record);
		}
		return record;
	}
};

// 创意监测
class AdsMonitorCreative
{

};

#endif
/* vim: set ts=4 sw=4 noet: */
