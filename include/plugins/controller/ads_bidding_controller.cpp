#include "plugins/controller/ads_bidding_controller.h"

#include <list>

#include "plugins/exchange/ads_exchange.h"
#include "core/advertise/ads_advertise_types.h"
#include "core/bidding/ads_bidding_param.h"
#include "core/bidding/ads_bidding_service.h"

using std::list;

int AdsBiddingController::process(AdsThreadData* p_thd_data)
{
	AdsHttpRequest *request = p_thd_data->request;
	AdsHttpResponse *response = p_thd_data->response;
	AdsBiddingParam param;
	
	// 获取exchange实例
	AdsExchange* exchange = getExchange( request->getUri(1) );
	if ( exchange == NULL ) {
		return ADS_HTTP_NOT_FOUND;
	}

	// 解析请求数据
	bool ret = exchange->parseBiddingRequest(request, param);
	if ( !ret ) {
		// 解析失败
		exchange->packBiddingResponse(param, NULL, response);
		return ADS_HTTP_BAD_REQUEST;
	}

	// 访问竞价服务接口
	AdsBiddingService& biddingService = getBiddingService();
	AdsAdvertise* ad = biddingService.bidding(&param);

	// 封装响应数据
	exchange->packBiddingResponse(param, ad, response);

	return ADS_HTTP_OK;
}