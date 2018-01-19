#ifndef _ADS_ADVERTISE_SERVICE_H
#define _ADS_ADVERTISE_SERVICE_H

/**
 * 广告模块对外服务接口类
 */

#include <list>
#include "core/ads_singleton.h"
#include "core/advertise/ads_advertise_types.h"

using std::list;

class AdsAdvertiseManager;

class AdsAdvertiseService : public AdsSingleton<AdsAdvertiseService>
{
	friend class AdsSingleton<AdsAdvertiseService>;
private:
	AdsAdvertiseService();

public:
	~AdsAdvertiseService();

	bool init();
	inline void reload();

	/* 检索当前可投放广告 */
	void search(list<AdsAdvertise*>& adList);

private:
	AdsAdvertiseManager *_manager; // 数据管理类
};

inline static AdsAdvertiseService& getAdvertiseService()
{
	return AdsAdvertiseService::getInstance();
}

#endif
/* vim: set ts=4 sw=4 noet: */
