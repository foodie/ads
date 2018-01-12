#ifndef _ADS_EXCHANGE_FACTORY_H
#define _ADS_EXCHANGE_FACTORY_H

#include <string>
#include <unordered_map>
#include "plugins/exchange/ads_exchange.h"

using std::string;
using std::unordered_map;

class AdsExchangeFactory
{
public:

	static bool init();

	static AdsExchange* getExchange(const string& name);

private:

	static unordered_map<string, AdsExchange*> *exchanges;

};

#endif
/* vim: set ts=4 sw=4 noet: */
