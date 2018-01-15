#include "plugins/exchange/ads_exchange.h"

#include "plugins/exchange/ads_exchange_factory.h"

AdsExchange* getExchange(const string& name)
{
	AdsExchangeFactory* factory = AdsExchangeFactory::getInstance();
	return factory->getExchange(name);
}