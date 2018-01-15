#ifndef _ADS_BIDDING_TYPES_H
#define _ADS_BIDDING_TYPES_H

#include "core/bidding/ads_bidding.h"

class AdsBiddingImpression
{
public:
	enum class Type { 
		BANNER = 1,
		VIDEO,
		NATIVE,
	};

	class Banner
	{
	private:
		int width;
		int height;
		int pos;
	};

	class Video
	{
	private:
		int width;
		int height;
		int minduration;
		int maxduration;
	};

private:
	string id;
	string zone_id; // 广告位
	
};

class AdsBiddingDevice
{
public:
	class Builder
	{
	public:
		
	};

private:
	string ip;

	AdsDeviceType 		type;
	AdsOs 				os;
	AdsCarrier 			carrier;
	AdsConnectionType 	connection_type;

	string idfa;	// 原值
	string imei;	// 大写,md5
	string mac;		// 无分隔符,md5
	string mac1;	// 有分隔符,md5
};

class AdsBiddingApp
{
private:
	string name;
};

class AdsBiddingUser
{
private:
	string exchange_id;	// 平台方id
};


#endif
/* vim: set ts=4 sw=4 noet: */
