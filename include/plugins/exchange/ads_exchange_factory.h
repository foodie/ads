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
	AdsExchangeFactory();
	~AdsExchangeFactory();

	AdsExchange* getExchange(const string& name) const;

private:
	unordered_map<string, AdsExchange*> *exchanges;

// static //
public:
	static AdsExchangeFactory* getInstance() 
	{
		return instance;
	}
private:
	static AdsExchangeFactory *instance;
};

#endif
/* vim: set ts=4 sw=4 noet: */
