#include "plugins/controller/ads_bidding_controller.h"

#include <list>

#include "plugins/exchange/ads_exchange.h"
#include "core/advertise/ads_advertise_types.h"
#include "core/advertise/ads_advertise_search.h"
#include "core/bidding/ads_bidding_param.h"
#include "core/bidding/ads_bidding_filter.h"
#include "core/bidding/ads_bidding_evaluate.h"
#include "core/bidding/ads_bidding_select.h"

using std::list;

int AdsBiddingController::process(AdsThreadData* p_thd_data)
{
	AdsHttpRequest *request = p_thd_data->request;
	AdsHttpResponse *response = p_thd_data->response;

	AdsBiddingParam param;
	AdsExchange* exchange = getExchange( request->getUri(1) );
	if ( exchange == NULL ) {
		return ADS_HTTP_NOT_FOUND;
	}

	bool ret = exchange->parseBiddingRequest(request, param);
	if ( !ret ) {
		// 解析失败
		exchange->packBiddingResponse(param, NULL, response);
		return ADS_HTTP_BAD_REQUEST;
	}

	list<AdsAdvertise*> list;

	// search
	AdsAdvertiseSearch::search(param, list);

	// filter
	AdsBiddingFilterResult filter;
	filter.filter(param, list);

	// evaluate

	// select
	AdsAdvertise* ad;

	exchange->packBiddingResponse(param, ad, response);

	return ADS_HTTP_OK;
}