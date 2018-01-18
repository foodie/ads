#ifndef _ADS_ADVERTISE_TYPES_H
#define _ADS_ADVERTISE_TYPES_H

#include <sys/time.h>
#include "core/ads_core.h"
#include "core/advertise/ads_advertise_material.h"

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <unordered_set>

using std::string;
using std::vector;
using std::list;
using std::unordered_map;
using std::unordered_set;

class AdsCampaign;
class AdsLaunch;
class AdsCreative;
class AdsAdvertiseCollection;

typedef AdsCreative AdsAdvertise;

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

	/* 所属关系 */
	list<AdsLaunch*> 	 launchs;
};

class AdsLaunch
{
public:
	int id;

	int 	bidding_type;
	int 	price;
	string 	pmp_id;

	/* 频次控制 */
	unsigned int imp_freq;	// 展示频次
	unsigned int clk_freq;	// 点击频次

	time_t 	start_date;
	time_t 	end_date;

	AdsAdvertiseSchedule 	schedule;	// 排期
	vector<int> 			budget;		// 预算
	AdsAdvertiseSpeed		speed;		// 速度

	int exchangeid;

	/* 定向 */
	unordered_set<int> 		area; 				// 地域
	AdsOsOrientation 		os; 				// 操作系统
	AdsDeviceOrientation 	device; 			// 设备
	unordered_set<int> 		connection_type; 	// 联网方式
	unordered_set<int> 		carrier;			// 运营商

	/* 所属关系 */
	AdsCampaign *campaign;
	list<AdsCreative*> creatives;
};

class AdsCreative
{

public:
	int id;

	string zone_id;

	int 			clk_act;
	string 			landing;
	vector<string> 	imp_track;
	vector<string> 	clk_track;
	

	/* 有效时间 */
	int 	effective;
	time_t 	start_date;
	time_t 	end_date;

	/* 所属关系 */
	AdsCampaign *campaign;
	AdsLaunch 	*launch;
	AdsMaterial *material;
};

/**
 * @brief      List of ads advertises.
 */
class AdsAdvertiseCollection
{
public:
	AdsAdvertiseCollection() {}
	~AdsAdvertiseCollection() { clear(); }

	/* adder */
	AdsCampaign* addCampaign(int id)
	{
		AdsCampaign *campaign = new (std::nothrow) AdsCampaign;
		campaigns.emplace(id, campaign);
		return campaign;
	}

	AdsLaunch* addLaunch(int id)
	{
		AdsLaunch* launch = new (std::nothrow) AdsLaunch;
		launchs.emplace(id, launch);
		return launch;
	}

	AdsCreative* addCreative(int id)
	{
		AdsCreative* creative = new (std::nothrow) AdsCreative;
		creatives.emplace(id, creative);
		return creative;
	}

	AdsMaterial* addMaterial(int id, AdsMaterialType type)
	{
		AdsMaterial* material = new (std::nothrow) AdsMaterial(type);
		materials.emplace(id, material);
		return material;
	}

	/* getter */
	AdsCampaign* getCampaign(int id)
	{
		auto itr = campaigns.find(id);
		if ( itr == campaigns.end() ) {
			return NULL;
		}
		return itr->second;
	}
	void getAllCampaigns(list<AdsCampaign*>& list)
	{
		for (auto itr : campaigns) {
			list.push_back(itr.second);
		}
	}

	AdsLaunch* getLaunch(int id)
	{
		auto itr = launchs.find(id);
		if ( itr == launchs.end() ) {
			return NULL;
		}
		return itr->second;
	}

	AdsCreative* getCreative(int id)
	{
		auto itr = creatives.find(id);
		if ( itr == creatives.end() ) {
			return NULL;
		}
		return itr->second;
	}

	AdsMaterial* getMaterial(int id)
	{
		auto itr = materials.find(id);
		if ( itr == materials.end() ) {
			return NULL;
		}
		return itr->second;
	}


	void clear()
	{
		// campaigns
		for ( auto itr = campaigns.begin(); itr != campaigns.end(); 
			itr = campaigns.erase(itr) ) {
			delete itr->second;
		}

		// launchs
		for ( auto itr = launchs.begin(); itr != launchs.end(); 
			itr = launchs.erase(itr) ) {
			delete itr->second;
		}

		// creatives
		for ( auto itr = creatives.begin(); itr != creatives.end(); 
			itr = creatives.erase(itr) ) {
			delete itr->second;
		}

		// materials
		for ( auto itr = materials.begin(); itr != materials.end(); 
			itr = materials.erase(itr) ) {
			delete itr->second;
		}
	}

private:

	unordered_map<int, AdsCampaign*> campaigns;
	unordered_map<int, AdsLaunch*> 	 launchs;
	unordered_map<int, AdsCreative*> creatives;
	unordered_map<int, AdsMaterial*> materials;
};

#endif
/* vim: set ts=4 sw=4 noet: */
