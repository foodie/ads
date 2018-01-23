#ifndef _ADS_ADVERTISE_LOADER_H
#define _ADS_ADVERTISE_LOADER_H

/**
 * 广告数据加载类
 */

#include <list>
#include "core/advertise/ads_advertise_types.h"
#include "utils/ads_curl.h"
#include "utils/ads_json.h"

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
	virtual void load(AdsAdvertiseCollection* collection) override
	{
		AdsCurl curl;
		curl.setRequestUrl("http://123.57.174.226/Admin/Api/returnAdQuery");
		curl.setRequestMethod(ADS_HTTP_POST);
		if ( !curl.execute() ) {
			WARN("[Advertise] Query advertise data failed");
			return;
		}
		const string& body = curl.getResponseBody();

		rapidjson::Document doc;
		if (doc.Parse( body.c_str() ).HasParseError()) {
			WARN("letv bid data parse failed");
			return;
		}

		AdsCampaign *campaign = collection->addCampaign(1);
	
		AdsLaunch *launch = collection->addLaunch(1);
		launch->campaign = collection->getCampaign(1);
		launch->campaign->launchs.push_back(launch);
	}
};

#endif
/* vim: set ts=4 sw=4 noet: */
