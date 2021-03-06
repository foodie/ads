#ifndef _ADS_ADVERTISE_LOADER_H
#define _ADS_ADVERTISE_LOADER_H

/**
 * 广告数据加载类
 */

#include <list>
#include "core/advertise/ads_advertise_types.h"

using std::list;

class AdsAdvertiseLoader
{
public:
	virtual void load(AdsAdvertiseCollection* collection) = 0;
};

// Mysql加载
class AdsAdvertiseMysqlLoader : public AdsAdvertiseLoader
{
public:
	virtual void load(AdsAdvertiseCollection* collection) override
	{

	}
};

// Api加载
class AdsAdvertiseApiLoader : public AdsAdvertiseLoader
{
public:
	virtual void load(AdsAdvertiseCollection* collection) override;
};

#endif
/* vim: set ts=4 sw=4 noet: */
