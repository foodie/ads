#ifndef _ADS_MONITOR_SERVICE_H
#define _ADS_MONITOR_SERVICE_H

/**
 * 监测模块对外服务接口类
 */
#include <string>
#include "time.h"
#include "core/ads_singleton.h"

using std::string;

class AdsMonitorManager;
class AdsMonitorParam;

class AdsMonitorService : public AdsSingleton<AdsMonitorService>
{
	friend class AdsSingleton<AdsMonitorService>;
public:
	~AdsMonitorService();

	bool init();

	/* 查询接口 */

	// 活动(总数据)
	unsigned int getCampaignTotalCost(int id); 	// 总预算
	unsigned int getCampaignTotalImp(int id); 	// 总展示数
	unsigned int getCampaignTotalClk(int id); 	// 总点击数
	
	unsigned int getCampaignImpFreq(int id, const string& devid, time_t ts=0);	// 展示频次
	unsigned int getCampaignClkFreq(int id, const string& devid, time_t ts=0);	// 点击频次

	// 投放(当日数据)
	unsigned int getLaunchTodayCost(int id); 			// 当天消耗金额
	unsigned int getLaunchTodayImp(int id); 			// 当天展示数
	unsigned int getLaunchTodayClk(int id); 			// 当天点击数

	unsigned int getLaunchImpFreq(int id, const string& devid, time_t ts=0);	// 展示
	unsigned int getLaunchClkFreq(int id, const string& devid, time_t ts=0);	// 点击数

	/* 更新接口 */
	void update(const AdsMonitorParam* param);

private:
	AdsMonitorService();

	AdsMonitorManager *_manager;
};

inline static AdsMonitorService& getMonitorService()
{
	return AdsMonitorService::getInstance();
}

#endif
/* vim: set ts=4 sw=4 noet: */
