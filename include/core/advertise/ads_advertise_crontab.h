#ifndef _ADS_ADVERTISE_CRONTAB_H
#define _ADS_ADVERTISE_CRONTAB_H

#include "plugins/crontab/ads_crontab_task.h"
#include "core/advertise/ads_advertise_service.h"

/**
 * @brief      Class for ads advertise reload task.
 */
class AdsAdvertiseReloadTask : public AdsCrontabTask
{
public:
	virtual void run() override
	{
		AdsAdvertiseService& service = AdsAdvertiseService::getInstance();
		service.reload();
		INFO("[Advertise] Advertise reload data success");
	}
};

#endif
/* vim: set ts=4 sw=4 noet: */
