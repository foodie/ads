#ifndef _ADS_EXCHANGE_FACTORY_H
#define _ADS_EXCHANGE_FACTORY_H

#include <string>
#include <unordered_map>
#include "core/ads_singleton.h"

using std::string;
using std::unordered_map;

class AdsExchange;

class AdsExchangeFactory : public AdsSingleton<AdsExchangeFactory>
{
	friend class AdsSingleton<AdsExchangeFactory>;
public:
	~AdsExchangeFactory();

	AdsExchange* getExchange(const string& name) const;

private:
	AdsExchangeFactory();

	unordered_map<string, AdsExchange*> *exchanges;
};

#endif
/* vim: set ts=4 sw=4 noet: */
