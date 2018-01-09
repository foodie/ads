#ifndef _ADS_MONITOR_SERVICE_H
#define _ADS_MONITOR_SERVICE_H


/**
 * 提供给其他模块的接口
 */

class AdsMonitorService
{
public:

	// 活动
	static bool checkCampaignBudget(int id, int budget); // 总预算
	static bool checkCampaignImp(int id, int imp); // 展示上限
	static bool checkCampaignClk(int id, int imp); // 点击上限
	static bool checkCampaignImpFreq(int id, int freq); // 展示频次
	static bool checkCampaignClkFreq(int id, int freq); // 点击频次

	// 投放
	static bool checkLaunchBudget(int id, int budget); // 预算
	static bool checkLaunchImpFreq(int id, int freq); // 展示频次
	static bool checkLaunchClkFreq(int id, int freq); // 点击频次

	// 创意

};

#endif
/* vim: set ts=4 sw=4 noet: */
