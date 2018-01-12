#ifndef _ADS_ADVERTISE_MANAGER_H
#define _ADS_ADVERTISE_MANAGER_H

#include <unordered_map>

/**
 * 数据只读
 */
class AdsAdvertiseCollection
{
public:
	
	unordered_map<int, AdsCampaign*> campaigns;
	unordered_map<int, AdsLaunch*> 	 launchs;
	unordered_map<int, AdsCreative*> creatives;
	unordered_map<int, AdsMaterial*> materials;

};

class AdsAdvertiseManager
{
public:
	static AdsAdvertiseManager* getInstance();

	int load();
	int reload();

private:
	int no;
	AdsAdvertiseCollection collections[2];
};

#endif
/* vim: set ts=4 sw=4 noet: */
