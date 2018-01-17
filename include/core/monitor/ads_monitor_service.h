#ifndef _ADS_MONITOR_SERVICE_H
#define _ADS_MONITOR_SERVICE_H

/**
 * 监测模块对外服务接口类
 */
#include "core/ads_singleton.h"

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
	int getCampaignCost(int id); 	// 总预算
	int getCampaignImp(int id); 	// 展示数
	int getCampaignClk(int id); 	// 点击数
	int getCampaignImpFreq(int id); // 展示频次
	int getCampaignClkFreq(int id); // 点击频次

	// 投放(当日数据)
	int getLaunchCost(int id); 		// 消耗金额
	int getLaunchImpFreq(int id); 	// 展示数
	int getLaunchClkFreq(int id); 	// 点击数

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
