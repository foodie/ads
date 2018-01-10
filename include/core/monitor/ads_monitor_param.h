#ifndef _ADS_MONITOR_PARAM_H
#define _ADS_MONITOR_PARAM_H

/**
 * 基本监测参数
 */

enum class AdsMonitorType { WINNOTICE, IMPRESSION, CLICK };

class AdsMonitorPARAM
{
public:
	AdsMonitorType type;

	// 广告信息
	int exchange_id;		// 媒体id
	int campaign_id;		// 活动id
	int launch_id;			// 投放id
	int creative_id;		// 创意id
	int material_id;		// 素材id

	// 设备信息
	int os;					// 操作系统
	int device_type;		// 设备类型

	string ua;
	string idfa;			// idfa
	string openudid;		// openudid
	string imei;			// imei MD5
	string androidid;		// android id MD5
	string androidid1;		// android id 
	string mac;				// mac MD5
	string mac1;			// mac1 MD5

	long ip;				// ip
	int address_id;			// 地域id

	// 应用信息
	string appname;			// 应用名称

};

#endif
/* vim: set ts=4 sw=4 noet: */
