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

typedef vector<time_t> AdsMonitorTimeRecord;
typedef phashmap<string, AdsMonitorTimeRecord*> AdsMonitorRecord;

class AdsMonitorDataBase
{
public:
	AdsMonitorDataBase()
		: _write_ts(0),_read_ts(0),
		  _imp_before(0),_clk_before(0),_cost_before(0),
		  _imp_today(0),_clk_today(0),_cost_today(0),
		  _imp_record(MAX_MONITOR_BITEMS_SIZE),
		  _clk_record(MAX_MONITOR_BITEMS_SIZE)
	{}

	~AdsMonitorDataBase()
	{
		for ( auto itr = _imp_record.begin(); itr != _imp_record.end(); itr++ ) {
			AdsMonitorTimeRecord *record = itr->second;
			delete record;
			_imp_record.erase(itr->first);
		}
		for ( auto itr = _clk_record.begin(); itr != _clk_record.end(); itr++ ) {
			AdsMonitorTimeRecord *record = itr->second;
			delete record;
			_clk_record.erase(itr->first);
		}
	}

public:

	/* 修改 */
	void setImpBefore(unsigned int imp)
	{
		_imp_before = imp;
	}
	void setClkBefore(unsigned int clk)
	{
		_clk_before = clk;
	}
	void setCostBefore(unsigned int cost)
	{
		_cost_before = cost;
	}

	void setImpToday(unsigned int imp)
	{
		_imp_today = imp;
	}
	void setClkToday(unsigned int clk)
	{
		_clk_today = clk;
	}
	void setCostToday(unsigned int cost)
	{
		_cost_today = cost;
	}

	void setImpInc()
	{
		checkDayPass();
		_imp_today++;
		refreshWriteTime();
	}
	void setClkInc()
	{
		checkDayPass();
		_clk_today++;
		refreshWriteTime();
	}
	void setCostInc(unsigned int c)
	{
		checkDayPass();
		_cost_today+=c;
		refreshWriteTime();
	}

	void addImpRecord(const string& id, time_t ts)
	{
		AdsMonitorTimeRecord *record;
		if ( _imp_record.get(id, &record) == lib::HASH_NOEXIST ) {
			record = new (std::nothrow) AdsMonitorTimeRecord;
			if ( record == NULL ) {
				WARN("new monitor time record failed");
				return;
			}
			_imp_record.set(id, record);
		}
		record->push_back(ts);
		refreshWriteTime();
	}
	void addClkRecord(const string& id, time_t ts)
	{
		AdsMonitorTimeRecord *record;
		if ( _clk_record.get(id, &record) == lib::HASH_NOEXIST ) {
			record = new (std::nothrow) AdsMonitorTimeRecord;
			if ( record == NULL ) {
				WARN("new monitor time record failed");
				return;
			}
			_clk_record.set(id, record);
		}
		record->push_back(ts);
		refreshWriteTime();
	}

	/* 查询 */
	unsigned int getImpTotal() 
	{
		refreshReadTime(); 
		return _imp_before + _imp_today; 
	}
	unsigned int getClkTotal() 
	{
		refreshReadTime(); 
		return _clk_before + _clk_today; 
	}
	unsigned int getCostTotal() 
	{
		refreshReadTime(); 
		return _cost_before + _cost_today; 
	}

	unsigned int getImpToday() 
	{
		refreshReadTime(); 
		return _imp_today; 
	}
	unsigned int getClkToday() 
	{
		refreshReadTime(); 
		return _clk_today; 
	}
	unsigned int getCostToday() 
	{
		refreshReadTime(); 
		return _cost_today; 
	}

	unsigned int getImpRecordNum(const string& id, time_t ts)
	{
		AdsMonitorTimeRecord *record;
		if ( _imp_record.get(id, &record) == lib::HASH_NOEXIST ) {
			return 0;
		}
		if ( ts == 0 ) {
			return record->size();
		}
		int sum = 0;
		for ( auto itr = record->rbegin(); itr != record->rend(); itr++ ) {
			if ( *itr < ts ) {
				break;
			}
			sum++;
		}
		return sum;
	}
	unsigned int getClkRecordNum(const string& id, time_t ts)
	{
		AdsMonitorTimeRecord *record;
		if ( _imp_record.get(id, &record) == lib::HASH_NOEXIST ) {
			return 0;
		}
		if ( ts == 0 ) {
			return record->size();
		}
		int sum = 0;
		for ( auto itr = record->rbegin(); itr != record->rend(); itr++ ) {
			if ( *itr < ts ) {
				break;
			}
			sum++;
		}
		return sum;
	}

private:
	time_t _write_ts; // 最后修改时间
	time_t _read_ts; // 最后读取时间

	// 历史数据
	unsigned int _imp_before;
	unsigned int _clk_before;
	unsigned int _cost_before;

	// 本日数据
	unsigned int _imp_today;
	unsigned int _clk_today;
	unsigned int _cost_today;

	AdsMonitorRecord _imp_record;	// 展示记录
	AdsMonitorRecord _clk_record;	// 点击记录

	void refreshWriteTime() { _write_ts = ads_nowtime(); }
	void refreshReadTime() { _read_ts = ads_nowtime(); }

	void checkDayPass() {
		if ( _write_ts < ads_today() ) {
			_imp_before  += _imp_today;
			_clk_before  += _clk_today;
			_cost_before += _cost_today;

			_imp_today  = 0;
			_clk_today  = 0;
			_cost_today = 0;
			
			_write_ts = ads_today();
		}
	}
};


#endif
/* vim: set ts=4 sw=4 noet: */
