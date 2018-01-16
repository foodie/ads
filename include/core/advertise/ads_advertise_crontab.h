#ifndef _ADS_ADVERTISE_CRONTAB_H
#define _ADS_ADVERTISE_CRONTAB_H

#include "plugins/crontab/ads_crontab_task.h"
#include "core/advertise/ads_advertise_manager.h"

/**
 * @brief      Class for ads advertise reload task.
 */
class AdsAdvertiseReloadTask : public AdsCrontabTask
{
public:
	virtual void run() override
	{
		AdsAdvertiseManager *manager = AdsAdvertiseManager::getInstance();
		manager->reload();
	}
};

#endif
/* vim: set ts=4 sw=4 noet: */
