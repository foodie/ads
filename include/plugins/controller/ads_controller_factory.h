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
	static bool init();
	
	static AdsController* getController(const string &name);
private:
	static unordered_map<string, AdsController*> *controllers;
};

#endif
/* vim: set ts=4 sw=4 noet: */
