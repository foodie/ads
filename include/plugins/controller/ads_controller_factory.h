#ifndef _ADS_CONTROLLER_FACTORY_H
#define _ADS_CONTROLLER_FACTORY_H

#include <string>
#include <unordered_map>
#include "core/ads_singleton.h"

using std::string;
using std::unordered_map;

class AdsController;

class AdsControllerFactory : public AdsSingleton<AdsControllerFactory>
{
	friend class AdsSingleton<AdsControllerFactory>;
public:
	~AdsControllerFactory();
	
	bool init();
	AdsController* getController(const string& name) const;
private:
	AdsControllerFactory();

	unordered_map<string, AdsController*> *controllers;
};

#endif
/* vim: set ts=4 sw=4 noet: */
