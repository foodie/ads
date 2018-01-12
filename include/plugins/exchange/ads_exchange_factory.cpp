#include "plugins/exchange/ads_exchange_factory.h"

#include "log.h"

#include "utils/ads_string.h"
#include "plugins/exchange/ads_adview_exchange.h"

unordered_map<string, AdsExchange*>* AdsExchangeFactory::exchanges = new unordered_map<string, AdsExchange*>;

bool AdsExchangeFactory::init()
{
	if ( exchanges == NULL ) {
		WARN("Exchanges is null");
		return false;
	}

	// adview
	AdsAdviewExchange *adview = new AdsAdviewExchange;
	if ( adview == NULL ) {
		WARN("Adview exchange is null");
		return false;
	}
	exchanges->emplace("adview", adview);

	return true;
}

AdsExchange* AdsExchangeFactory::getExchange(const string& name)
{
	if ( exchanges != NULL ) {
		ads_string_tolower(name);
		auto itr = exchanges->find(name);
		if ( itr != exchanges->end() ) {
			return itr->second;
		}
	}
	return NULL;
}
