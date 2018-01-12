#ifndef _ADS_ADVERTISE_TYPES_H
#define _ADS_ADVERTISE_TYPES_H

#include <sys/time.h>
#include "advertise/ads_advertise_material.h"

class AdsCampaign
{
public:
	int id;

	unsigned int budget; 	// 预算,分
	unsigned int imp_limit; // 展示上限
	unsigned int clk_limit; // 点击上限
	
	/* 频次控制 */
	unsigned int imp_freq;	// 展示频次
	unsigned int clk_freq;	// 点击频次

	time_t 	start_date;
	time_t 	end_date;
};

class AdsLaunch
{
public:
	int id;

	int bidding_type;
	int price;


};

class AdsCreative
{

public:
	int id;

	string ad_zone_id;

	int 			clk_act;
	string 			landing;
	vector<string> 	imp_track;
	vector<string> 	clk_track;
	

	/* 有效时间 */
	int 	effective;
	time_t 	start_date;
	time_t 	end_date;
};


class AdsAdvertise
{
public:

	AdsCampaign *campaign;
	AdsLaunch 	*launch;
	AdsCreative *creative;
	AdsMaterial *material;

};

#endif
/* vim: set ts=4 sw=4 noet: */
