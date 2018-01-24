#include "plugins/controller/ads_bidding_controller.h"

#include <iostream>
#include <list>
#include "log.h"
#include "plugins/exchange/ads_exchange.h"
#include "core/advertise/ads_advertise_types.h"
#include "core/advertise/ads_advertise_service.h"
#include "core/bidding/ads_bidding_param.h"
#include "core/bidding/ads_bidding_service.h"
#include "utils/ads_ip_location.h"

using std::list;
using std::cout;
using std::endl;

#define LOG_BIDDING_PARAM 0

static void log_bidding_param(AdsBiddingParam& param);

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

	// 通过ip获取地域id
	auto deviceBuilder = param.device().getBuilder();
	deviceBuilder.setAddressId(0);

	if ( LOG_BIDDING_PARAM ) {
		log_bidding_param(param);
	}

	// 获取可投放广告列表
	list<AdsAdvertise*> adList;
	AdsAdvertiseService& adService = getAdvertiseService();
	adService.search(adList);

	// 竞价处理
	AdsBiddingService& biddingService = getBiddingService();
	AdsAdvertise* ad = biddingService.bidding(param, adList);

	// 封装响应数据
	exchange->packBiddingResponse(param, ad, response);

	return ADS_HTTP_OK;
}

static void log_bidding_param(AdsBiddingParam& param)
{
	DEBUG("Log Bidding Param");
	cout << "---------- Bidding Param ----------" << endl;
	cout << "ExchangeId: " << param.exchangeId() << endl;
	cout << "RequestId: " << param.biddingId() << endl;
	
	auto imp = param.impression();
	cout << "  [impression]" << endl;
	cout << "  id: " 		 << imp.id() << endl;
	cout << "  zoneid: " 	 << imp.zoneId() << endl;
	cout << "  type: " 		 << imp.type() << endl;
	cout << "  settlement: " << imp.settlement() << endl;
	cout << "  bidFloor: " 	 << imp.bidFloor() << endl;
	cout << "  bidType: " 	 << imp.biddingType() << endl;
	switch( imp.type() ) {
		case AdsAdvertiseType::BANNER:
		case AdsAdvertiseType::PLAQUE:
		case AdsAdvertiseType::SPLASH:
		{			
			auto banner = imp.banner();
	cout << "    [banner]" << endl;
	cout << "    width: " 	<< banner.width() << endl;
	cout << "    height: " 	<< banner.height() << endl;
	cout << "    pos: " 	<< banner.pos() << endl;
			break;
		}
		case AdsAdvertiseType::VIDEO:
		{	
			auto video = imp.video();
	cout << "    [video]" << endl;
	cout << "    width: " 	<< video.width() << endl;
	cout << "    height: " 	<< video.height() << endl;
	cout << "    minDuration: "	<< video.minDuration() << endl;
	cout << "    maxDuration: " << video.maxDuration() << endl;
			break;
		}
		case AdsAdvertiseType::NATIVE:
	cout << "    [native]" << endl;
			break;
	}
	if ( imp.biddingType() == AdsBiddingType::PDB ) {
		auto pmp = imp.pmp();
	cout << "    [pmp]" << endl;
	cout << "    id: " << pmp.id() << endl;
	}
	
	auto device = param.device();
	cout << "  [device]" << endl;
	cout << "  type: " 			 << device.type() << endl;
	cout << "  os: " 			 << device.os() << endl;
	cout << "  carrier: " 		 << device.carrier() << endl;
	cout << "  connectionType: " << device.connectionType() << endl;
	cout << "  ip: " 			 << device.ip() << endl;
	cout << "  ua: " 			 << device.ua() << endl;
	cout << "  idfa: " 			 << device.idfa() << endl;
	cout << "  imei: " 			 << device.imei() << endl;
	cout << "  mac: " 			 << device.mac() << endl;
	cout << "  addressId: " 	 << device.addressId() << endl;

	auto app = param.app();
	cout << "  [app]" << endl;
	cout << "  name: " << app.name() << endl;

	auto user = param.user();
	cout << "  [user]" << endl;
	cout << "  exchangeId: " << user.exchangeId() << endl;

	cout << "------------------------------------" << endl;
}