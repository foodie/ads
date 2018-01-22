#include "plugins/exchange/ads_exchange_factory.h"

#include "log.h"
#include "utils/ads_string.h"
#include "plugins/exchange/ads_exchange.h"
#include "plugins/exchange/ads_adview_exchange.h"

AdsExchangeFactory::AdsExchangeFactory()
{
}

AdsExchangeFactory::~AdsExchangeFactory()
{
	if ( exchanges != NULL ) {
		for ( auto itr = exchanges->begin(); itr != exchanges->end();
			itr = exchanges->erase(itr) ) {
			delete itr->second;
			itr->second = NULL;
		}
	}
}

bool AdsExchangeFactory::init()
{
	// 构造hash map
	exchanges = new unordered_map<string, AdsExchange*>;
	if ( exchanges == NULL ) {
		WARN("Exchanges is null");
		return false;
	}

	// 添加exchange实例
	exchanges->emplace("adview", new AdsAdviewExchange);

	INFO("[Exchange] Exchange plugin init success");
	return true;
}

AdsExchange* AdsExchangeFactory::getExchange(const string& name) const
{
	if ( exchanges != NULL ) {
		auto itr = exchanges->find( ads_string_tolower(name) );
		if ( itr != exchanges->end() ) {
			return itr->second;
		}
	}
	return NULL;
}
