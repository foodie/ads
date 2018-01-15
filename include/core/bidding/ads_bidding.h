#ifndef _ADS_BIDDING_H
#define _ADS_BIDDING_H

enum class AdsBiddingType
{
	RTB = 1,
	PDB
};

enum class AdsBiddingImpressionType
{
	BANNER = 1,

	VIDEO,
	NATIVE,
};

enum class AdsDeviceType
{
	UNKNOW = 0,
	MOBILE = 1,
	PAD = 2,
};

enum class AdsOs
{
	UNKNOW = 0,
	IOS = 1,
	ANDROID,
};

enum class AdsCarrier
{
	CHINA_MOBILE = 1, 	// 移动
	CHINA_UNICOM, 		// 联通
	CHINA_TELECOM, 		// 电信
};

enum class AdsConnectionType
{
	PC = 1,
	WIFI,
	GSM_UNKNOW,
	GSM_2G,
	GSM_3G,
	GSM_4G,
};

#endif
/* vim: set ts=4 sw=4 noet: */
