#ifndef _ADS_CORE_H
#define _ADS_CORE_H

#define ADS_OK  		0
#define ADS_ERROR 		-1

static const unsigned int MAX_HOST_LEN = 256;
static const unsigned int MAX_HOST_PORT_LEN = 256;

static const unsigned int MAX_NODE_NAME_LEN = 32;

static const unsigned int MAX_FNAME_LEN = 256;
static const unsigned int MAX_FPATH_LEN = 256;

static const unsigned int MAX_BUF_LEN = 2048;


enum class AdsAdvertiseSchedule
{
	AVERAGE = 2,
	CUSTOM,
};

enum class AdsAdvertiseSpeed
{
	AVERAGE = 1,
	FAST,
};

enum class AdsOsOrientation
{
	NONE = 0,
	IOS,
	ANDROID,
};

enum class AdsDeviceOrientation
{
	NONE = 0,
	MOBILE,
	PAD,
};

enum class AdsBiddingType
{
	RTB = 1,
	PDB
};

enum class AdsBiddingSettlement
{
	CPM = 1,
	CPC,
};

enum class AdsImpressionType
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
	UNKNOW = 0,
	CHINA_MOBILE = 1, 	// 移动
	CHINA_UNICOM, 		// 联通
	CHINA_TELECOM, 		// 电信
};

enum class AdsConnectionType
{
	UNKNOW = 0,
	PC = 1,
	WIFI,
	GSM_UNKNOW,
	GSM_2G,
	GSM_3G,
	GSM_4G,
};

#endif
/* vim: set ts=4 sw=4 noet: */
