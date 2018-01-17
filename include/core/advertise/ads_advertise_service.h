#ifndef _ADS_ADVERTISE_SERVICE_H
#define _ADS_ADVERTISE_SERVICE_H

/**
 * 广告模块对外服务接口类
 */

#include <list>
#include "core/ads_singleton.h"

using std::list;

class AdsAdvertise;
class AdsAdvertiseManager;

class AdsAdvertiseService : public AdsSingleton<AdsAdvertiseService>
{
	friend class AdsSingleton<AdsAdvertiseService>;
private:
	AdsAdvertiseService();

public:
	~AdsAdvertiseService();

	inline void reload();

	/* 检索广告 */
	void search(list<AdsAdvertise*>& list);
	AdsAdvertise* search(id);

private:
	AdsAdvertiseManager *_manager;
};

inline static AdsAdvertiseService& getAdvertiseService()
{
	return AdsAdvertiseService::getInstance();
}

#endif
/* vim: set ts=4 sw=4 noet: */
