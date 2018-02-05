#include "core/advertise/ads_advertise_service.h"

#include "log.h"
#include "utils/ads_time.h"
#include "plugins/crontab/ads_crontab.h"
#include "core/advertise/ads_advertise_manager.h"
#include "core/advertise/ads_advertise_loader.h"
#include "core/advertise/ads_advertise_crontab.h"
#include "core/monitor/ads_monitor_service.h"

static bool checkCampaign(AdsCampaign* campaign);
static bool checkLaunch(AdsLaunch* launch);
static bool checkCreative(AdsCreative* creative);

AdsAdvertiseService::AdsAdvertiseService()
{
}

AdsAdvertiseService::~AdsAdvertiseService()
{
	if ( _manager != NULL ) {
		delete _manager;
		_manager = NULL;
	}
}


bool AdsAdvertiseService::init()
{
	// 构造数据管理实例
	_manager = new (std::nothrow) AdsAdvertiseManager(new AdsAdvertiseApiLoader);
	if ( _manager == NULL ) {
		WARN("Advertise Manager is null");
		return false;
	}
	_manager->load();

	// 添加定时任务
	registerCrontabTask("0,30 * * * *", new AdsAdvertiseReloadTask);

	INFO("[Advertise] Advertise Service init success");
	return true;
}


void AdsAdvertiseService::reload()
{
	_manager->reload();
}

void AdsAdvertiseService::search(list<AdsAdvertise*>& al, int eid)
{
	AdsAdvertiseCollection *collection = _manager->get();

	// 当前时间戳
	time_t nowtime = ads_nowtime();

	// 获取所有活动列表
	list<AdsCampaign*> campaigns;
	collection->getAllCampaigns(campaigns);

	// 检查
	for ( AdsCampaign* campaign : campaigns ) {
		if ( !checkCampaign(campaign) ) {
			continue;
		}
			
		for ( AdsLaunch* launch : campaign->launchs ) {
			if ( !checkLaunch(launch) ) {
				continue;
			}

			if ( launch->exchangeid != eid ) {
				continue;
			}
			
			for ( AdsCreative* creative : launch->creatives ) {
				if ( !checkCreative(creative) ) {
					continue;
				}

				al.push_back(creative);
			}
		}
	}

}

static bool checkCampaign(AdsCampaign* campaign)
{
	// 时间
	time_t nowtime = ads_nowtime();
	if ( campaign->start_date > nowtime || campaign->end_date < nowtime ) {
		return false;
	}

	AdsMonitorService& monitorServ = getMonitorService();
	// 预算
	unsigned int cost = monitorServ.getCampaignTotalCost(campaign->id);
	if ( cost >= campaign->budget ) {
		return false;
	}
	// 展示上限
	if ( campaign->imp_limit > 0 ) {
		unsigned int imp = monitorServ.getCampaignTotalImp(campaign->id);
		if ( imp >= campaign->imp_limit ) {
			return false;
		}
	}
	// 点击上限
	if ( campaign->clk_limit > 0 ) {
		unsigned int clk = monitorServ.getCampaignTotalClk(campaign->id);
		if ( clk >= campaign->clk_limit ) {
			return false;
		}
	}

	return true;
}

static bool checkLaunch(AdsLaunch* launch)
{
	// 时间
	time_t nowtime = ads_nowtime();
	if ( launch->start_date > nowtime || launch->end_date < nowtime ) {
		return false;
	}

	// 排期
	unsigned int budget = 0;
	if ( launch->schedule == AdsAdvertiseSchedule::AVERAGE ) {
		// 平均
		budget = launch->budget.size() > 0 ? launch->budget[0] : 0;
	} else if ( launch->schedule == AdsAdvertiseSchedule::CUSTOM ) {
		// 自定义
		int idx = (nowtime - launch->start_date) / (24*3600);
		budget = launch->budget.size() > idx ? launch->budget[idx] : 0;
	}
	// 投放速度
	if ( launch->speed == AdsAdvertiseSpeed::AVERAGE ) {
		// 平均
		int hour = nowtime / 3600 % 24 + 8; // 0-23
		budget = budget * (hour + 1) / 24; 
	} 
	/*
	else if ( launch->speed == AdsAdvertiseSpeed::FAST ) {
		// 快速
	}
	*/
	AdsMonitorService& monitorServ = getMonitorService();
	unsigned int cost = monitorServ.getLaunchTodayCost(launch->id);
	if ( cost >= budget ) {
		return false;
	}

	return true;
}

static bool checkCreative(AdsCreative* creative)
{
	// 时间
	if ( creative->effective == 1 ) {
		time_t nowtime = ads_nowtime();
		if ( creative->start_date > nowtime || creative->end_date < nowtime ) {
			return false;
		}
	}
	
	return true;
}