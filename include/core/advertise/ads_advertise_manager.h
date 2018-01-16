#ifndef _ADS_ADVERTISE_MANAGER_H
#define _ADS_ADVERTISE_MANAGER_H

#include <memory>
#include <unordered_map>
#include "core/advertise/ads_advertise_types.h"
#include "core/advertise/ads_advertise_loader.h"

using std::unique_ptr;
using std::unordered_map;

class AdsAdvertiseManager
{
public:
	AdsAdvertiseManager(AdsAdvertiseLoader *loader) 
		: _loader(loader),
		  _no(0)
	{
		_collections[0] = new AdsAdvertiseCollection;
		_collections[1] = new AdsAdvertiseCollection;

		_loader->load( _collections[0] );
	}

	void reload()
	{
		// 在备份内存中重载广告数据
		AdsAdvertiseCollection* c2 = _collections[ 1 - _no];
		c2->clear();
		_loader->load(c2);

		// 交换主备内存
		_no = 1 - _no;
	}

	AdsAdvertiseCollection* get() const
	{
		return _collections[_no];
	}

private:
	unique_ptr<AdsAdvertiseLoader> _loader;

	int _no;
	AdsAdvertiseCollection* _collections[2];

public:
	static AdsAdvertiseManager* getInstance()
	{
		return instance;
	}
private:
	static AdsAdvertiseManager* instance;
};

AdsAdvertiseManager* AdsAdvertiseManager::instance = new AdsAdvertiseManager(new AdsAdvertiseApiLoader);

#endif
/* vim: set ts=4 sw=4 noet: */
