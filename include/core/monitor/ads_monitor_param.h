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
	int campaign_id;
	int launch_id;
	int creative_id;
	int material_id;

	// 设备信息
	int os;
	int device_type;

	string ua;
	string 	idfa;			// idfa
	string 	openudid;		// openudid
	string 	imei;			// imei MD5
	string 	androidid;		// android id MD5
	string 	androidid1;		// android id 
	string 	mac;			// mac MD5
	string 	mac1;			// mac1 MD5

	long ip;
	int address_id;

	// 应用信息
	string appname;

};

#endif
/* vim: set ts=4 sw=4 noet: */
