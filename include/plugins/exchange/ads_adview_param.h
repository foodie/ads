#ifndef _ADS_ADVIEW_PARAM_H
#define _ADS_ADVIEW_PARAM_H

#define ADVIEW_AT_HIGHEST 	0
#define ADVIEW_AT_SECOND 	1
#define ADVIEW_AT_FIXED 	2

class AdsAdviewBanner
{
public:
	int w;
	int h;
	int pos;
};

class AdsAdviewNative
{
};

class AdsAdviewVideo
{
public:
	int w;
	int h;
	int minduration;
	int maxduration;
};

class AdsAdviewPmp
{
public:
	string id;
	int bidfloor;
	int at;
};

class AdsAdviewImpression
{
public:
	string id;
	int instl; // 0-横幅 1-插屏 4-开屏 5-视频 6-原生
	string tagid;
	int bidfloor;
	AdsAdviewBanner banner;
	AdsAdviewNative native;
	AdsAdviewVideo video;
	AdsAdviewPmp pmp;
};

class AdsAdviewBidRequest
{
public:
	string id; // 广告请求id
	int at; // 0-最高价成交 1-次高价成交 2-优先购买
	AdsAdviewImpression imp; // 广告描述(只取1个)
};


#endif
/* vim: set ts=4 sw=4 noet: */
