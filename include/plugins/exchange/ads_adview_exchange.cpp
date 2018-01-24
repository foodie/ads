#include "plugins/exchange/ads_adview_exchange.h"

#include "log.h"
#include "core/ads_core.h"
#include "utils/ads_json.h"

#define ADVIEW_COMMON_ADZONE_ID  "adview_000000"

bool AdsAdviewExchange::parseBiddingRequest(AdsHttpRequest *request, 
	AdsBiddingParam& param)
{
	rapidjson::Document doc;
	if (doc.Parse( request->getBody().c_str() ).HasParseError()) {
		WARN("[Exchange] AdviewExchange parse bidding request failed");
		return false;
	}

	auto builder = param.getBuilder();
	// exchangeid
	builder.setExchangeId( ADVIEW );
	// biddingid
	auto biddingidItr = doc.FindMember("id");
	if ( biddingidItr != doc.MemberEnd() ) {
		builder.setBiddingId( biddingidItr->GetString() );
	}

	// imp
	auto impBuilder = param.impression().getBuilder();
	auto impItr = doc.FindMember("imp");
	if ( impItr != doc.MemberEnd() ) {
		const rapidjson::Value& imps = impItr->value;
		if ( imps.IsArray() && imps.Size() > 0 ) {
			const rapidjson::Value& imp = imps[0];
			// id
			auto idItr = imp.FindMember("id");
			if ( idItr != imp.MemberEnd() ) {
				impBuilder.setId( idItr->GetString() );
			}
			// zoneid
			impBuilder.setZoneId( ADVIEW_COMMON_ADZONE_ID );

			// type
			auto typeItr = imp.FindMember("instl");
			if ( typeItr != imp.MemberEnd() ) {
				if ( typeItr->IsInt() ) {
					switch(typeItr->GetInt()) {
						case 0: // 横幅
							impBuilder.setType(AdsAdvertiseType::BANNER); 
							break;
						case 1: // 插屏
							impBuilder.setType(AdsAdvertiseType::PLAQUE); 
							break;
						case 4: // 开屏
							impBuilder.setType(AdsAdvertiseType::SPLASH); 
							break;
						case 5: // 视频
							impBuilder.setType(AdsAdvertiseType::VIDEO); 
							break;
						case 6: // 原生
							impBuilder.setType(AdsAdvertiseType::NATIVE); 
							break;
					}
				}
			}
			// settlement
			// bidFloor
			auto bfItr = imp.FindMember("bidfloor");
			if ( bfItr != imp.MemberEnd() ) {
				if ( bfItr->IsInt() ) {
					impBuilder.setBidFloor( bfItr->GetInt() );
				}
			}

			// banner
			auto bannerBuilder = param.impression().banner().getBuilder();
			auto bannerItr = imp.FindMember("banner");
			if ( bannerItr != imp.MemberEnd() ) {
				const rapidjson::Value& banner = bannerItr->value;
				if ( banner.IsObject() ) {
					// width
					auto widthItr = banner.FindMember("w");
					if ( widthItr != banner.MemberEnd() ) {
						if ( widthItr->IsInt() ) {
							bannerBuilder.setWidth( widthItr->GetInt() );
						}
					}
					// height
					auto heightItr = banner.FindMember("h");
					if ( heightItr != banner.MemberEnd() ) {
						if ( heightItr->IsInt() ) {
							bannerBuilder.setHeight( heightItr->GetInt() );
						}
					}
					// pos
					auto posItr = banner.FindMember("pos");
					if ( posItr != banner.MemberEnd() ) {
						if ( posItr->IsInt() ) {
							bannerBuilder.setPos( posItr->GetInt() );
						}
					}
				}
			}

			// video
			auto videoBuilder = param.impression().video().getBuilder();
			auto videoItr = imp.FindMember("video");
			if ( videoItr != imp.MemberEnd() ) {
				const rapidjson::Value& video = videoItr->value;
				if ( video.IsObject() ) {
					// width
					auto widthItr = video.FindMember("w");
					if ( widthItr != video.MemberEnd() ) {
						if ( widthItr->IsInt() ) {
							videoBuilder.setWidth( widthItr->GetInt() );
						}
					}
					// height
					auto heightItr = video.FindMember("h");
					if ( heightItr != video.MemberEnd() ) {
						if ( heightItr->IsInt() ) {
							videoBuilder.setHeight( heightItr->GetInt() );
						}
					}
					// minDuration
					auto mindItr = video.FindMember("minduration");
					if ( mindItr != video.MemberEnd() ) {
						if ( mindItr->IsInt() ) {
							videoBuilder.setMinDuration( mindItr->GetInt() );
						}
					}
					// maxDuration
					auto maxdItr = video.FindMember("maxduration");
					if ( maxdItr != video.MemberEnd() ) {
						if ( maxdItr->IsInt() ) {
							videoBuilder.setMaxDuration( maxdItr->GetInt() );
						}
					}
				}
			}

			// native

			// biddingType
			// pmp
			auto pmpBuilder = param.impression().pmp().getBuilder();
			auto pmpItr = imp.FindMember("pmp");
			if ( pmpItr != imp.MemberEnd() ) {
				impBuilder.setBiddingType(AdsBiddingType::PDB);
				const rapidjson::Value& pmp = pmpItr->value;
				if ( pmp.IsObject() ) {
					auto dealItr = pmp.FindMember("deals")
					if ( dealItr != pmp.MemberEnd() ) {
						const rapidjson::Value& deals = dealItr->value;
						if ( deals.IsArray() && deals.Size() > 0 ) {
							const rapidjson::Value& deal = deals[0];
							// id
							auto dealidItr = deal.FindMember("id");
							if ( dealidItr != deal.MemberEnd() ) {
								if ( dealidItr->IsString() ) {
									pmpBuilder.setId( dealidItr->GetString() );
								}
							}
						}
					}
				}
			}
		}
	}

	// device
	auto deviceBuilder = param.device().getBuilder();
	auto deviceItr = doc.FindMember("device");
	if ( deviceItr != doc.MemberEnd() ) {
		const rapidjson::Value& device = deviceItr->value;
		// type
		auto typeItr = device.FindMember("devicetype");
		if ( typeItr != device.MemberEnd() ) {
			if ( typeItr->IsInt() ) {
				switch(typeItr->GetInt()) {
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
			if ( osItr->IsString() ) {
				string os = ads_string_toupper( osItr->GetString() );
				if ( os == "IOS" ) {
					deviceBuilder.setOs( AdsOs::IOS );
				} else if ( os == "ANDROID" ) {
					deviceBuilder.setOs( AdsOs::ANDROID );
				} else {
					deviceBuilder.setOs( AdsOs::UNKNOW );
				}
			}
		}
		// carrier
		auto carrierItr = device.FindMember("carrier");
		if ( carrierItr != device.MemberEnd() ) {
			if ( carrierItr->IsString() ) {
				int carrier = ads_string_to_int( carrierItr->GetString() );
				switch( carrier ) {
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
			if ( ctItr->IsInt() ) {
				switch( ctItr->GetInt() ) {
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
			if ( ipItr->IsString() ) {
				deviceBuilder.setIp( ipItr->GetString() );
			}
		}
		// ua
		auto uaItr = device.FindMember("ua");
		if ( uaItr != device.MemberEnd() ) {
			if ( uaItr->IsString() ) {
				deviceBuilder.setUa( uaItr->GetString() );
			}
		}
		// idfa
		// imei
		AdsOs os = param.device().os();
		if ( os == AdsOs::IOS ) {
			// idfa
			auto idfaItr = device.FindMember("ifa");
			if ( idfaItr != device.MemberEnd() ) {
				if ( idfaItr->IsString() ) {
					deviceBuilder.setIdfa( idfaItr->GetString() );
				}
			} else if ( (idfaItr = device.FindMember("dpidmd5")) != device.MemberEnd() ) {
				if ( idfaItr->IsString() ) {
					deviceBuilder.setIdfa( idfaItr->GetString() );
				}
			} else if ( (idfaItr = device.FindMember("dpidsha1")) != device.MemberEnd() ) {
				if ( idfaItr->IsString() ) {
					deviceBuilder.setIdfa( idfaItr->GetString() );
				}
			}
		} else if ( os == AdsOs::ANDROID ) {
			// imei
			auto imeiItr = device.FindMember("didmd5");
			if ( imeiItr != device.MemberEnd() ) {
				if ( imeiItr->IsString() ) {
					deviceBuilder.setImei( imeiItr->GetString() );
				}
			} else if ( (imeiItr = device.FindMember("didsha1")) != device.MemberEnd() ) {
				if ( imeiItr->IsString() ) {
					deviceBuilder.setImei( imeiItr->GetString() );
				}
			}
		}
		// mac
		auto macItr = device.FindMember("macmd5");
		if ( macItr != device.MemberEnd() ) {
			if ( macItr->IsString() ) {
				deviceBuilder.setMac( macItr->GetString() );
			}
		} else if ( (macItr = device.FindMember("macsha1")) != device.MemberEnd() ) {
			if ( macItr->IsString() ) {
				deviceBuilder.setMac( macItr->GetString() );
			}
		}
		// mac1
	}

	// app
	auto appBuilder = param.app().getBuilder();
	auto appItr = doc.FindMember("app");
	if ( appItr != doc.MemberEnd() ) {
		const rapidjson::Value& app = appItr->value;
		// name
		auto nameItr = app.FindMember("name");
		if ( nameItr != app.MemberEnd() ) {
			if ( nameItr->IsString() ) {
				appBuilder.setName( nameItr->GetString() );
			}
		}
	}

	// user
	auto userBuilder = param.user().getBuilder();
	auto userItr = doc.FindMember("user");
	if ( userItr != doc.MemberEnd() ) {
		const rapidjson::Value& user = userItr->value;
		// id
		auto idItr = user.FindMember("id");
		if ( idItr != user.MemberEnd() ) {
			if ( idItr->IsString() ) {
				userBuilder.setExchangeId( idItr->GetString() );
			}
		}
	}

	return true;
}

void AdsAdviewExchange::packBiddingResponse(AdsBiddingParam& param,
	AdsAdvertise *ad, AdsHttpResponse *response)
{

}

int AdsAdviewExchange::decryptWinPrice(const string& str)
{
	return 0;	
}