#ifndef _ADS_DEF_H
#define _ADS_DEF_H

#include <string>
#include <serv.h>
#include <signal.h>
#include <gflags/gflags.h>

#include "conf.h"
extern "C" {
#include <ini_config.h>
}

#define ADS_OK  		0
#define ADS_ERROR 		-1

// ads cookie的键名
#define ADS_COOKIE_KEY					"ID"

// 监测接口
#define ADS_MONITOR_WINNOTICE_API		"monitor/winnotice" 
#define ADS_MONITOR_IMPRESSION_API		"monitor/impression"
#define ADS_MONITOR_CLICK_API			"monitor/click"


// 监测数据中的键名
// bid
#define ADS_MONITOR_KEY_AD_ID 			"a"
#define ADS_MONITOR_KEY_AD_PLAN_ID 		"p"
#define ADS_MONITOR_KEY_AD_GROUP_ID 	"g"
#define ADS_MONITOR_KEY_AD_STUFF_ID 	"s"
#define ADS_MONITOR_KEY_COST_MODE		"cm"

#define ADS_MONITOR_KEY_ADX_ID 			"x"
#define ADS_MONITOR_KEY_BID_MODE		"bm"
#define ADS_MONITOR_KEY_AD_TAG 			"t"

#define ADS_MONITOR_KEY_BID_ID 			"b"
#define ADS_MONITOR_KEY_TIMESTAMP		"ts"

#define ADS_MONITOR_KEY_PRICE 			"pr"
#define ADS_MONITOR_KEY_REDIRECT		"re"
// device
#define ADS_MONITOR_KEY_IP				"ip"
#define ADS_MONITOR_KEY_UA				"ua"
#define ADS_MONITOR_KEY_DEVICE			"dev"
#define ADS_MONITOR_KEY_OS				"os"
	// IDFA 原始值
#define ADS_MONITOR_KEY_IDFA			"m1"
	// OPENUDID 原始值
#define ADS_MONITOR_KEY_OPENUDID		"m2"
	// IMEI MD5
#define ADS_MONITOR_KEY_IMEI			"m3"
	// Android ID MD5
#define ADS_MONITOR_KEY_ANDROIDID		"m4"
	// Android ID 原始值
#define ADS_MONITOR_KEY_ANDROIDID1		"m4a"
	// MAC 大写去分隔符 MD5
#define ADS_MONITOR_KEY_MAC				"ma"
	// MAC 大写保留分隔符 MD5
#define ADS_MONITOR_KEY_MAC1			"ma1"
// app
#define ADS_MONITOR_KEY_APP_KEY			"ak"
#define ADS_MONITOR_KEY_APP_NAME		"an"
// user
#define ADS_MONITOR_KEY_ADS_UID			"u1"
#define ADS_MONITOR_KEY_ADX_UID			"u2"
#define ADS_MONITOR_KEY_MEDIA_UID		"u3"

// 流量来源
#define ADX_ADVIEW 						1
#define ADX_SNMI						2
#define ADX_MOJI 						5

// 购买方式
#define ADS_RTB							1
#define ADS_PDB							2

// 计费方式
#define ADS_CPM							1
#define ADS_CPC							2

// 广告位展示类型
#define ADS_AD_BANNER 					1
#define ADS_AD_INTERSTITIAL				2
#define ADS_AD_SPLASH					3
#define ADS_AD_NATIVE					4
#define ADS_AD_VIDEO					5

// 素材类型
#define ADS_STUFF_IMAGE					1
#define ADS_STUFF_HTML5					4
#define ADS_STUFF_VIDEO					6
#define ADS_STUFF_NATIVE				8

// 点击事件类型
#define ADS_CLICK_OPEN_WEBPAGE 			1
#define	ADS_CLICK_DOWNLOAD_APP			2
#define	ADS_CLICK_OPEN_MAP				3
#define	ADS_CLICK_SEND_SMS				4
#define	ADS_CLICK_SEND_EMAIL			5
#define	ADS_CLICK_CALL					6
#define	ADS_CLICK_PLAY_VIDEO			7

// 设备类型
#define	ADS_DEVICE_PHONE				2
#define	ADS_DEVICE_PAD					3


// 操作系统类型
#define ADS_OS_UNKNOW					0
#define ADS_OS_IOS  					1
#define ADS_OS_ANDROID 					2

#endif
/* vim: set ts=4 sw=4 noet: */
