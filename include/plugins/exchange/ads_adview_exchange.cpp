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
	builder.setExchangeId( ADVIEW );
	if ( (auto idItr = doc.FindMember("id")) != doc.MemberEnd() ) {
		builder.setBiddingId( idItr->GetString() );
	}

	// imp
	auto impBuilder = param.impression().getBuilder();
	if ( (auto impItr = doc.FindMember("imp")) != doc.MemberEnd() ) {
		const rapidjson::Value& imps = impItr->value;
		if ( imps.IsArray() && imps.Size() > 0 ) {
			const rapidjson::Value& imp = imps[0];

			// id
			if ( (auto idItr = imp.FindMember("id")) != imp.MemberEnd() ) {
				impBuilder.setId( idItr->GetString() );
			}
			// zoneid
			impBuilder.setZoneId( ADVIEW_COMMON_ADZONE_ID );

			// type
			if ( (auto typeItr = imp.FindMember("instl")) != imp.MemberEnd() ) {
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
			if ( (auto bfItr = imp.FindMember("bidfloor")) != imp.MemberEnd() ) {
				if ( bfItr->IsInt() ) {
					impBuilder.setBidFloor( bfItr->GetInt() );
				}
			}

			// banner
			auto bannerBuilder = param.impression().banner().getBuilder();
			if ( (auto bannerItr = imp.FindMember("banner")) != imp.MemberEnd() ) {
				const rapidjson::Value& banner = bannerItr->value;
				if ( banner.IsObject() ) {
					// width
					if ( (auto widthItr = banner.FindMember("w")) != banner.MemberEnd() ) {
						if ( widthItr->IsInt() ) {
							bannerBuilder.setWidth( widthItr->GetInt() );
						}
					}
					// height
					if ( (auto heightItr = banner.FindMember("h")) != banner.MemberEnd() ) {
						if ( heightItr->IsInt() ) {
							bannerBuilder.setHeight( heightItr->GetInt() );
						}
					}
					// pos
					if ( (auto posItr = banner.FindMember("pos")) != banner.MemberEnd() ) {
						if ( posItr->IsInt() ) {
							bannerBuilder.setPos( posItr->GetInt() );
						}
					}
				}
			}

			// video
			auto videoBuilder = param.impression().video().getBuilder();
			if ( (auto videoItr = imp.FindMember("video")) != imp.MemberEnd() ) {
				const rapidjson::Value& video = videoItr->value;
				if ( video.IsObject() ) {
					// width
					if ( (auto widthItr = video.FindMember("w")) != video.MemberEnd() ) {
						if ( widthItr->IsInt() ) {
							videoBuilder.setWidth( widthItr->GetInt() );
						}
					}
					// height
					if ( (auto heightItr = video.FindMember("h")) != video.MemberEnd() ) {
						if ( heightItr->IsInt() ) {
							videoBuilder.setHeight( heightItr->GetInt() );
						}
					}
					// minDuration
					if ( (auto mindItr = video.FindMember("minduration")) != video.MemberEnd() ) {
						if ( mindItr->IsInt() ) {
							videoBuilder.setMinDuration( mindItr->GetInt() );
						}
					}
					// maxDuration
					if ( (auto maxdItr = video.FindMember("maxduration")) != video.MemberEnd() ) {
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
			if ( (auto pmpItr = imp.FindMember("pmp")) != imp.MemberEnd() ) {
				impBuilder.setBiddingType(AdsBiddingType::PDB);
				const rapidjson::Value& pmp = pmpItr->value;
				if ( pmp.IsObject() ) {
					if ( (auto dealItr = pmp.FindMember("deals")) != pmp.MemberEnd() ) {
						const rapidjson::Value& deals = dealItr->value;
						if ( deals.IsArray() && deals.Size() > 0 ) {
							const rapidjson::Value& deal = deals[0];
							// id
							if ( (auto idItr = deal.FindMember("id")) != deal.MemberEnd() ) {
								if ( idItr->IsString() ) {
									pmpBuilder.setId( idItr->GetString() );
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
	if ( (auto deviceItr = doc.FindMember("device")) != doc.MemberEnd() ) {
		const rapidjson::Value& device = deviceItr->value;
		// type
		if ( (auto typeItr = device.FindMember("devicetype")) != device.MemberEnd() ) {
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
		if ( (auto osItr = device.FindMember("os")) != device.MemberEnd() ) {
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
		if ( (auto carrierItr = device.FindMember("carrier")) != device.MemberEnd() ) {
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
		if ( (auto ctItr = device.FindMember("connectiontype")) != device.MemberEnd() ) {
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
		if ( (auto ipItr = device.FindMember("ip")) != device.MemberEnd() ) {
			if ( ipItr->IsString() ) {
				deviceBuilder.setIp( ipItr->GetString() );
			}
		}
		// ua
		if ( (auto uaItr = device.FindMember("ua")) != device.MemberEnd() ) {
			if ( uaItr->IsString() ) {
				deviceBuilder.setUa( uaItr->GetString() );
			}
		}
		// idfa
		// imei
		AdsOs os = param.device().os();
		if ( os == AdsOs::IOS ) {
			// idfa
			if ( (auto idfaItr = device.FindMember("ifa")) != device.MemberEnd() ) {
				if ( idfaItr->IsString() ) {
					deviceBuilder.setIdfa( idfaItr->GetString() );
				}
			} else if ( (auto idfaItr = device.FindMember("dpidmd5")) != device.MemberEnd() ) {
				if ( idfaItr->IsString() ) {
					deviceBuilder.setIdfa( idfaItr->GetString() );
				}
			} else if ( (auto idfaItr = device.FindMember("dpidsha1")) != device.MemberEnd() ) {
				if ( idfaItr->IsString() ) {
					deviceBuilder.setIdfa( idfaItr->GetString() );
				}
			}
		} else if ( os == AdsOs::ANDROID ) {
			// imei
			if ( (auto imeiItr = device.FindMember("didmd5")) != device.MemberEnd() ) {
				if ( imeiItr->IsString() ) {
					deviceBuilder.setImei( imeiItr->GetString() );
				}
			} else if ( (auto imeiItr = device.FindMember("didsha1")) != device.MemberEnd() ) {
				if ( imeiItr->IsString() ) {
					deviceBuilder.setImei( imeiItr->GetString() );
				}
			}
		}
		// mac
		if ( (auto macItr = device.FindMember("macmd5")) != device.MemberEnd() ) {
			if ( macItr->IsString() ) {
				deviceBuilder.setMac( macItr->GetString() );
			}
		} else if ( (auto macItr = device.FindMember("macsha1")) != device.MemberEnd() ) {
			if ( macItr->IsString() ) {
				deviceBuilder.setMac( macItr->GetString() );
			}
		}
		// mac1
	}

	// app
	auto appBuilder = param.app().getBuilder();
	if ( (auto appItr = doc.FindMember("app")) != doc.MemberEnd() ) {
		const rapidjson::Value& app = appItr->value;
		// name
		if ( (auto nameItr = app.FindMember("name")) != app.MemberEnd() ) {
			if ( nameItr->IsString() ) {
				appBuilder.setName( nameItr->GetString() );
			}
		}
	}

	// user
	auto userBuilder = param.user().getBuilder();
	if ( (auto userItr = doc.FindMember("user")) != doc.MemberEnd() ) {
		const rapidjson::Value& user = userItr->value;
		// id
		if ( (auto idItr = user.FindMember("id")) != user.MemberEnd() ) {
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