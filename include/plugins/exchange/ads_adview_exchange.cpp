#include "plugins/exchange/ads_adview_exchange.h"

#include <sstream>
#include "log.h"
#include "ads_conf.h"
#include "core/ads_core.h"
#include "utils/ads_string.h"
#include "utils/ads_json.h"
#include "plugins/decrypter/ads_google_decrypter.h"

using std::ostringstream;

#define ADVIEW_COMMON_ADZONE_ID  "adview_000000"

#define ADVIEW_NBR_USER_UNMATCH 8

AdsAdviewExchange::AdsAdviewExchange()
{
}

AdsAdviewExchange::~AdsAdviewExchange()
{
}

/***************************************************************/

static bool parseImpression(const rapidjson::Value& rdoc, 
	AdsAdviewImpression& rimp);
static bool parseDevice(const rapidjson::Value& rdoc, 
	AdsBiddingDevice& rdevice);
static bool parseApp(const rapidjson::Value& rdoc, 
	AdsBiddingApp& rapp);


bool AdsAdviewExchange::parseBiddingRequest(AdsHttpRequest *request, 
		AdsBiddingParam& param, void *buf)
{
	AdsAdviewBidRequest *bidRequest = new (buf) AdsAdviewBidRequest;
	auto builder = param.getBuilder();

	builder.setExchangeId( ADVIEW );

	rapidjson::Document doc;
	if (doc.Parse( request->getBody().c_str() ).HasParseError()) {
		WARN("[Exchange] AdviewExchange parse bidding request failed");
		return false;
	}

	// id
	auto idItr = doc.FindMember("id");
	if ( idItr != doc.MemberEnd() ) {
		builder.setBiddingId( idItr->value.GetString() );
		bidRequest->id = idItr->value.GetString();
	}

	// at
	auto atItr = doc.FindMember("at");
	if ( atItr != doc.MemberEnd() ) {
		bidRequest->at = atItr->value.GetInt();
	}

	// imp
	if ( !parseImpression(doc, bidRequest->imp) ) {
		return false;
	}

	// device
	parseDevice(doc, param.device());

	// app
	parseApp(doc, param.app());

	// user

	return true;
}

static bool parseImpression(const rapidjson::Value& rdoc, 
	AdsAdviewImpression& rimp)
{
	auto impItr = rdoc.FindMember("imp");
	if ( impItr == rdoc.MemberEnd() ) {
		return false;
	}

	const rapidjson::Value& imps = impItr->value;
	if ( !imps.IsArray() || imps.Size() == 0 ) {
		return false;
	}
	
	const rapidjson::Value& imp = imps[0];

	// id
	auto idItr = imp.FindMember("id");
	if ( idItr != imp.MemberEnd() ) {
		rimp.id = idItr->value.GetString();
	}

	// instl
	auto instlItr = imp.FindMember("instl");
	if ( instlItr != imp.MemberEnd() ) {
		rimp.instl = instlItr->value.GetInt();
	}

	// tagid
	auto tagidItr = imp.FindMember("tagid");
	if ( tagidItr != imp.MemberEnd() ) {
		rimp.tagid = tagidItr->value.GetString();
	}
	
	// bidfloor
	auto bidfloorItr = imp.FindMember("bidfloor");
	if ( bidfloorItr != imp.MemberEnd() ) {
		rimp.bidfloor = bidfloorItr->value.GetInt();
	}
	
	// banner
	AdsAdviewBanner& rbanner = rimp.banner;
	auto bannerItr = imp.FindMember("banner");
	if ( bannerItr != imp.MemberEnd() ) {
		const rapidjson::Value& banner = bannerItr->value;
		// w
		auto wItr = banner.FindMember("w");
		if ( wItr != banner.MemberEnd() ) {
			rbanner.w = wItr->value.GetInt();
		}
		// h
		auto hItr = banner.FindMember("h");
		if ( hItr != banner.MemberEnd() ) {
			rbanner.h = hItr->value.GetInt();
		}
		// pos
		auto posItr = banner.FindMember("pos");
		if ( posItr != banner.MemberEnd() ) {
			rbanner.pos = posItr->value.GetInt();
		}
	}

	// video
	AdsAdviewVideo& rvideo = rimp.video;
	auto videoItr = imp.FindMember("video");
	if ( videoItr != imp.MemberEnd() ) {
		const rapidjson::Value& video = videoItr->value;
		// w
		auto wItr = video.FindMember("w");
		if ( wItr != video.MemberEnd() ) {
			rvideo.w = wItr->value.GetInt();
		}
		// h
		auto hItr = video.FindMember("h");
		if ( hItr != video.MemberEnd() ) {
			rvideo.h = hItr->value.GetInt();
		}
		// minduration
		auto mindurationItr = video.FindMember("minduration");
		if ( mindurationItr != video.MemberEnd() ) {
			rvideo.minduration = mindurationItr->value.GetInt();
		}
		// maxduration
		auto maxdurationItr = video.FindMember("maxduration");
		if ( maxdurationItr != video.MemberEnd() ) {
			rvideo.maxduration = maxdurationItr->value.GetInt();
		}
	}

	// native

	// pmp
	AdsAdviewPmp& rpmp = rimp.pmp;
	auto pmpItr = imp.FindMember("pmp");
	if ( pmpItr != imp.MemberEnd() ) {
		const rapidjson::Value& pmp = pmpItr->value;
		auto dealsItr = pmp.FindMember("deals");
		if ( dealsItr != pmp.MemberEnd() ) {
			const rapidjson::Value& deals = dealsItr->value;
			if ( deals.IsArray() && deals.Size() > 0 ) {
				const rapidjson::Value& deal = deals[0];
				// id
				auto idItr = deal.FindMember("id");
				if ( idItr != deal.MemberEnd() ) {
					rpmp.id = idItr->value.GetString();
				}
				// bidfloor
				auto bidfloorItr = deal.FindMember("bidfloor");
				if ( bidfloorItr != deal.MemberEnd() ) {
					rpmp.bidfloor = bidfloorItr->value.GetInt();
				}
				// at
				auto atItr = deal.FindMember("at");
				if ( atItr != deal.MemberEnd() ) {
					rpmp.at = atItr->value.GetInt();
				}
			}
		}
	}

	return true;
}

static bool parseDevice(const rapidjson::Value& rdoc, 
	AdsBiddingDevice& rdevice)
{
	auto deviceItr = rdoc.FindMember("device");
	if ( deviceItr == rdoc.MemberEnd() ) {
		return false;
	}

	const rapidjson::Value& device = deviceItr->value;
	auto deviceBuilder = rdevice.getBuilder();

	// type
	auto typeItr = device.FindMember("devicetype");
	if ( typeItr != device.MemberEnd() ) {
		const rapidjson::Value& type = typeItr->value;
		if ( type.IsInt() ) {
			switch( type.GetInt() ) {
				case 1: // iPhone
				case 2: // Android手机
				case 4: // Windows Phone
					deviceBuilder.setType( AdsDeviceType::MOBILE );
					break;
				case 3: // iPad
				case 5: // Android平板
					deviceBuilder.setType( AdsDeviceType::PAD );
					break;
				case 6: // 智能TV
				case 7: // PC
				default:
					deviceBuilder.setType( AdsDeviceType::UNKNOW );
					break;
			}
		}
	}

	// os
	auto osItr = device.FindMember("os");
	if ( osItr != device.MemberEnd() ) {
		const rapidjson::Value& os = osItr->value;
		if ( os.IsString() ) {
			string osStr = ads_string_toupper( os.GetString() );
			if ( osStr == "IOS" ) {
				deviceBuilder.setOs( AdsOs::IOS );
			} else if ( osStr == "ANDROID" ) {
				deviceBuilder.setOs( AdsOs::ANDROID );
			} else {
				deviceBuilder.setOs( AdsOs::UNKNOW );
			}
		}
	}
	
	// carrier
	auto carrierItr = device.FindMember("carrier");
	if ( carrierItr != device.MemberEnd() ) {
		const rapidjson::Value& carrier = carrierItr->value;
		if ( carrier.IsString() ) {
			int carrierInt = ads_string_to_int( carrier.GetString() );
			switch( carrierInt ) {
				case 46000:
				case 46002:
				case 46007:
					deviceBuilder.setCarrier( AdsCarrier::CHINA_MOBILE );
					break;
				case 46001:
				case 46006:
					deviceBuilder.setCarrier( AdsCarrier::CHINA_UNICOM );
					break;
				case 46003:
				case 46005:
					deviceBuilder.setCarrier( AdsCarrier::CHINA_TELECOM );
					break;
			}
		}
	}
	
	// connection_type
	auto ctItr = device.FindMember("connectiontype");
	if ( ctItr != device.MemberEnd() ) {
		const rapidjson::Value& connection_type = ctItr->value;
		if ( connection_type.IsInt() ) {
			switch( connection_type.GetInt() ) {
				case 1: // PC
					deviceBuilder.setConnectionType( AdsConnectionType::PC );
					break;
				case 2: // WIFI
					deviceBuilder.setConnectionType( AdsConnectionType::WIFI );
					break;
				case 3: // 蜂窝-未知
					deviceBuilder.setConnectionType( AdsConnectionType::GSM_UNKNOW );
					break;
				case 4: // 蜂窝-2G
					deviceBuilder.setConnectionType( AdsConnectionType::GSM_2G );
					break;
				case 5: // 蜂窝-3G
					deviceBuilder.setConnectionType( AdsConnectionType::GSM_3G );
					break;
				case 6: // 蜂窝-4G
					deviceBuilder.setConnectionType( AdsConnectionType::GSM_4G );
					break;
				case 0: // 未知
				default:
					deviceBuilder.setConnectionType( AdsConnectionType::UNKNOW );
					break;
			}
		}
	}
	
	// ip
	auto ipItr = device.FindMember("ip");
	if ( ipItr != device.MemberEnd() ) {
		const rapidjson::Value& ip = ipItr->value;
		if ( ip.IsString() ) {
			deviceBuilder.setIp( ip.GetString() );
		}
	}

	// ua
	auto uaItr = device.FindMember("ua");
	if ( uaItr != device.MemberEnd() ) {
		const rapidjson::Value& ua = uaItr->value;
		if ( ua.IsString() ) {
			deviceBuilder.setUa( ua.GetString() );
		}
	}

	// idfa
	// imei
	AdsOs os = rdevice.os();
	if ( os == AdsOs::IOS ) {
		// idfa
		auto idfaItr = device.FindMember("ifa");
		if ( idfaItr != device.MemberEnd() ) {
			if ( idfaItr->value.IsString() ) {
				deviceBuilder.setIdfa( idfaItr->value.GetString() );
			}
		} else if ( (idfaItr = device.FindMember("dpidmd5")) != device.MemberEnd() ) {
			if ( idfaItr->value.IsString() ) {
				deviceBuilder.setIdfa( idfaItr->value.GetString() );
			}
		} else if ( (idfaItr = device.FindMember("dpidsha1")) != device.MemberEnd() ) {
			if ( idfaItr->value.IsString() ) {
				deviceBuilder.setIdfa( idfaItr->value.GetString() );
			}
		}
	} else if ( os == AdsOs::ANDROID ) {
		// imei
		auto imeiItr = device.FindMember("didmd5");
		if ( imeiItr != device.MemberEnd() ) {
			if ( imeiItr->value.IsString() ) {
				deviceBuilder.setImei( imeiItr->value.GetString() );
			}
		} else if ( (imeiItr = device.FindMember("didsha1")) != device.MemberEnd() ) {
			if ( imeiItr->value.IsString() ) {
				deviceBuilder.setImei( imeiItr->value.GetString() );
			}
		}
	}

	// mac
	auto macItr = device.FindMember("macmd5");
	if ( macItr != device.MemberEnd() ) {
		if ( macItr->value.IsString() ) {
			deviceBuilder.setMac( macItr->value.GetString() );
		}
	} else if ( (macItr = device.FindMember("macsha1")) != device.MemberEnd() ) {
		if ( macItr->value.IsString() ) {
			deviceBuilder.setMac( macItr->value.GetString() );
		}
	}
	// mac1

	return true;
}

static bool parseApp(const rapidjson::Value& rdoc, 
	AdsBiddingApp& rapp)
{
	auto appItr = rdoc.FindMember("app");
	if ( appItr == rdoc.MemberEnd() ) {
		return false;
	}

	const rapidjson::Value& app = appItr->value;
	auto appBuilder = rapp.getBuilder();

	// name
	auto nameItr = app.FindMember("name");
	if ( nameItr != app.MemberEnd() ) {
		const rapidjson::Value& name = nameItr->value;
		if ( name.IsString() ) {
			appBuilder.setName( name.GetString() );
		}
	}

	return true;
}

/***************************************************************/

static bool checkAdvertise(AdsAdviewBidRequest& req, AdsAdvertise *ad);

void AdsAdviewExchange::biddingFilter(void *buf, list<AdsAdvertise*>& al)
{
	AdsAdviewBidRequest *bidRequest = (AdsAdviewBidRequest*) buf;

	for (auto itr = al.begin(); itr != al.end(); ) {
		if ( checkAdvertise(*bidRequest, *itr) ) {
			itr++;
		} else {
			itr = al.erase(itr);
		}
	}

}

static bool checkAdvertise(AdsAdviewBidRequest& req, AdsAdvertise *ad)
{
	

	return true;
}

/***************************************************************/
string AdsAdviewExchange::getWinnoticeUrl(AdsBiddingParam& param, 
		AdsAdvertise *ad)
{
	string url = AdsExchange::getWinnoticeUrl(param, ad);
	ostringstream oss;
	oss << url << "&" << ADS_MONITOR_KEY_PRICE << "=" << "%%WIN_PRICE%%";
	return oss.str();
}

string AdsAdviewExchange::getImpressionUrl(AdsBiddingParam& param, 
		AdsAdvertise *ad)
{
	string url = AdsExchange::getImpressionUrl(param, ad);
	ostringstream oss;
	oss << url << "&" << ADS_MONITOR_KEY_PRICE << "=" << "%%WIN_PRICE%%";
	return oss.str();
}

/***************************************************************/

void AdsAdviewExchange::packBiddingResponse(AdsBiddingParam& param, void *buf,
		AdsAdvertise *ad, AdsHttpResponse *response)
{
	AdsAdviewBidRequest *bidRequest = (AdsAdviewBidRequest*) buf;

	if ( ad == NULL ) {
		this->packBiddingFailure(param, response);
	} else {
		this->packBiddingSuccess(param, bidRequest, ad, response);
	}

	bidRequest->~AdsAdviewBidRequest();
}

void AdsAdviewExchange::packBiddingFailure(AdsBiddingParam& param, 
	AdsHttpResponse *response)
{
	rapidjson::Document doc;
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value root(rapidjson::kObjectType);

	root.AddMember("id", rapidjson::StringRef( param.biddingId().c_str() ), allocator); // BidRequest的唯一标识
	root.AddMember("nbr", ADVIEW_NBR_USER_UNMATCH, allocator);

	response->setBody( ads_json_to_string(root) );
}

void AdsAdviewExchange::packBiddingSuccess(AdsBiddingParam& param, 
	AdsAdviewBidRequest *req, AdsAdvertise *ad, AdsHttpResponse *response)
{
	rapidjson::Document doc;
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value root(rapidjson::kObjectType);

	// id string
	root.AddMember("id", rapidjson::StringRef( param.biddingId().c_str() ), allocator); // BidRequest的唯一标识
	// bidid string
	root.AddMember("bidid", rapidjson::StringRef( param.biddingId().c_str() ), allocator); // BidRequest的唯一标识
	// seatbid array
	rapidjson::Value seatbid(rapidjson::kArrayType);
	// [ SeatBid
	rapidjson::Value SeatBid(rapidjson::kObjectType);
	// 		bid array
	rapidjson::Value bid(rapidjson::kArrayType);
	//		[ Bid
	rapidjson::Value Bid(rapidjson::kObjectType);
	
	AdsAdviewImpression& imp = req->imp;
	//			impid
	Bid.AddMember("impid", rapidjson::StringRef( imp.id.c_str() ), allocator);
	//			price
	Bid.AddMember("price", ad->launch->price * 100, allocator);
	//			paymode
	switch( ad->launch->settlement ) {
		case AdsBiddingSettlement::CPM:
			Bid.AddMember("paymode", 1, allocator);
			break;
		case AdsBiddingSettlement::CPC:
			Bid.AddMember("paymode", 2, allocator);
			break;
	}
	//			adct
	switch( ad->action ) {
		case AdsClickAction::OPEN_WEBPAGE:
			Bid.AddMember("adct", 1, allocator);
			break;
		case AdsClickAction::DOWNLOAD_APP:
			Bid.AddMember("adct", 2, allocator);
			break;
	}
	//			adid
	//string adid = ads_int_to_string(ad->id);
	//Bid.AddMember("adid", rapidjson::StringRef( adid.c_str() ), allocator);

	switch( imp.instl ) {
		case 0:
		case 1:
		case 4:
		{
			AdsMaterialImage *img = ad->material->image();

			// admt
			Bid.AddMember("admt", 1, allocator);
			// adi
			string adi = this->getMaterialSourceUrl( img->source.c_str() );
			Bid.AddMember("adi", rapidjson::StringRef( adi.c_str() ), allocator);
			// adw
			Bid.AddMember("adw", img->width, allocator);
			// adh
			Bid.AddMember("adh", img->height, allocator);
		}
		break;
	}

	// 			adLogo
	Bid.AddMember("adLogo", rapidjson::StringRef( g_conf->adview.logo ), allocator);
	//			adurl
	if ( ad->imp_track.size() > 0 ) {
		string& adurl = ad->imp_track.at(0);
		Bid.AddMember("adurl", rapidjson::StringRef( adurl.c_str() ), allocator);
	} else {
		Bid.AddMember("adurl", rapidjson::StringRef( ad->landing.c_str() ), allocator);
	}
	//			wurl 赢价
	string winUrl = this->getWinnoticeUrl(param, ad);
	Bid.AddMember("wurl", rapidjson::StringRef( winUrl.c_str() ), allocator);
	//			nurl 展示
	rapidjson::Value nurl(rapidjson::kObjectType);
	rapidjson::Value nurl_0(rapidjson::kArrayType);
	if ( ad->imp_track.size() > 1 ) {
		for ( size_t i = 1; i < ad->imp_track.size(); i++ ) {
			string& impt = ad->imp_track.at(i);
			nurl_0.PushBack(rapidjson::StringRef( impt.c_str() ), allocator);
		}
	}
	string impUrl = this->getImpressionUrl(param, ad);
	nurl_0.PushBack(rapidjson::StringRef( impUrl.c_str() ), allocator);
	nurl.AddMember("0", nurl_0, allocator);
	Bid.AddMember("nurl", nurl, allocator);
	// 			curl 点击
	rapidjson::Value curl(rapidjson::kArrayType);
	for ( size_t i = 0; i < ad->clk_track.size(); i++ ) {
		string& clkt = ad->clk_track.at(i);
		curl.PushBack(rapidjson::StringRef( clkt.c_str() ), allocator);
	}
	string clkUrl = this->getClickUrl(param, ad);
	curl.PushBack(rapidjson::StringRef( clkUrl.c_str() ), allocator);
	Bid.AddMember("curl", curl, allocator);
	//			dealid
	if ( !imp.pmp.id.empty() ) {
		Bid.AddMember("dealid", rapidjson::StringRef( imp.pmp.id.c_str() ), allocator);
	}
	//			cid 创意id
	string cid = ads_int_to_string(ad->id);
	Bid.AddMember("cid", rapidjson::StringRef( cid.c_str() ), allocator);
	//			crid 物料id
	string crid = ads_int_to_string(ad->material->id());
	Bid.AddMember("crid", rapidjson::StringRef( crid.c_str() ), allocator);

	bid.PushBack(Bid, allocator);
	//		]
	SeatBid.AddMember("bid", bid, allocator);
	// 		seat string
	seatbid.PushBack(SeatBid, allocator);
	// ]
	root.AddMember("seatbid", seatbid, allocator);

	response->setBody( ads_json_to_string(root) );
}

/***************************************************************/

int AdsAdviewExchange::decryptWinPrice(const string& str)
{
	if ( str == "%%WIN_PRICE%%" ) {
		return 0;
	}
	int p = google_decrypt_winning_price(str.c_str(), 
		g_conf->adview.ekey, g_conf->adview.ikey);
	return p / 100;	
}


/***************************************************************
*
****************************************************************/