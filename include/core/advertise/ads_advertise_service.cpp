#include "core/advertise/ads_advertise_service.h"

#include "utils/ads_time.h"
#include "plugins/crontab/ads_crontab.h"
#include "core/advertise/ads_advertise_types.h"
#include "core/advertise/ads_advertise_manager.h"
#include "core/advertise/ads_advertise_loader.h"
#include "core/advertise/ads_advertise_crontab.h"

AdsAdvertiseService::AdsAdvertiseService()
{
	_manager = new AdsAdvertiseManager(new AdsAdvertiseApiLoader);
	_manager->load();

	// 添加定时任务
	//crontab.add("0 */30 * * *", new AdsAdvertiseReloadTask);
}

AdsAdvertiseService::~AdsAdvertiseService()
{
	if ( _manager != NULL ) {
		delete _manager;
		_manager = NULL;
	}
}

void AdsAdvertiseService::reload()
{
	_manager->reload();
}

void AdsAdvertiseService::search(list<AdsAdvertise*>& list)
{
	AdsAdvertiseCollection *collection = _manager->get();

	// 当前时间戳
	time_t nowtime = ads_nowtime();

	// 获取所有活动列表
	list<AdsCampaign*> campaigns;
	collection->getAllCampaigns(campaigns);

	for ( AdsCampaign* campaign : campaigns ) {
		// 过滤
			if ( campaign->start_date > nowtime 
				|| campaign->end_date < nowtime ) {
				continue;
			}
			
			for ( AdsLaunch* launch : campaign->launchs ) {
				// 过滤
				if ( launch->start_date > nowtime
					|| launch->end_date < nowtime ) {
					continue;
				}

				for ( AdsCreative* creative : launch->creatives ) {
					// 过滤
					if (creative->effective == 1) {
						if ( creative->start_date > nowtime 
							|| creative->end_date < nowtime ) {
							continue;
						}
					}
					list.push_back(creative);
				}
			}
		}
}