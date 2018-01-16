#ifndef _ADS_ADVERTISE_INITIALIZE_H
#define _ADS_ADVERTISE_INITIALIZE_H

#include "plugins/crontab/ads_crontab.h"
#include "core/advertise/ads_advertise_manager.h"
#include "core/advertise/ads_advertise_crontab.h"

/**
 * @brief      Class for ads advertise initialize.
 */
class AdsAdvertiseInitialize
{
public:
	
	void initialize()
	{
		// 加载
		AdsAdvertiseManager *manager = AdsAdvertiseManager::getInstance();
		manager->load();

		crontab.add("0 */30 * * *", new AdsAdvertiseReloadTask);
	}

};

#endif
/* vim: set ts=4 sw=4 noet: */
