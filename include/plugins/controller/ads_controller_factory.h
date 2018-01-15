#ifndef _ADS_CONTROLLER_FACTORY_H
#define _ADS_CONTROLLER_FACTORY_H

#include <string>
#include <unordered_map>
#include "plugins/controller/ads_controller.h"

using std::string;
using std::unordered_map;

class AdsControllerFactory
{
public:
	AdsControllerFactory();
	~AdsControllerFactory();
	
	AdsController* getController(const string& name) const;
private:
	unordered_map<string, AdsController*> *controllers;

// static //
public:
	static AdsControllerFactory* getInstance()
	{
		return instance;
	}
private:
	static AdsControllerFactory *instance;
};

#endif
/* vim: set ts=4 sw=4 noet: */
