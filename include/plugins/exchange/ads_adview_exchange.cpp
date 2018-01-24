#include "plugins/exchange/ads_adview_exchange.h"

#include "log.h"
#include "ads_conf.h"
#include "core/ads_core.h"
#include "utils/ads_string.h"
#include "utils/ads_json.h"
#include "plugins/decrypter/ads_google_decrypter.h"

#define ADVIEW_COMMON_ADZONE_ID  "adview_000000"

#define ADVIEW_NBR_USER_UNMATCH 8

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
		builder.setBiddingId( biddingidItr->value.GetString() );
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
				impBuilder.setId( idItr->value.GetString() );
			}
			// zoneid
			impBuilder.setZoneId( ADVIEW_COMMON_ADZONE_ID );

			// type
			auto typeItr = imp.FindMember("instl");
			if ( typeItr != imp.MemberEnd() ) {
				const rapidjson::Value& type = typeItr->value;
				if ( type.IsInt() ) {
					switch( type.GetInt() ) {
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
				const rapidjson::Value& bidfloor = bfItr->value;
				if ( bidfloor.IsInt() ) {
					impBuilder.setBidFloor( bidfloor.GetInt() );
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
						const rapidjson::Value& width = widthItr->value;
						if ( width.IsInt() ) {
							bannerBuilder.setWidth( width.GetInt() );
						}
					}
					// height
					auto heightItr = banner.FindMember("h");
					if ( heightItr != banner.MemberEnd() ) {
						const rapidjson::Value& height = heightItr->value;
						if ( height.IsInt() ) {
							bannerBuilder.setHeight( height.GetInt() );
						}
					}
					// pos
					auto posItr = banner.FindMember("pos");
					if ( posItr != banner.MemberEnd() ) {
						const rapidjson::Value& pos = posItr->value;
						if ( pos.IsInt() ) {
							bannerBuilder.setPos( pos.GetInt() );
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
						const rapidjson::Value& width = widthItr->value;
						if ( width.IsInt() ) {
							videoBuilder.setWidth( width.GetInt() );
						}
					}
					// height
					auto heightItr = video.FindMember("h");
					if ( heightItr != video.MemberEnd() ) {
						const rapidjson::Value& height = heightItr->value;
						if ( height.IsInt() ) {
							videoBuilder.setHeight( height.GetInt() );
						}
					}
					// minDuration
					auto mindItr = video.FindMember("minduration");
					if ( mindItr != video.MemberEnd() ) {
						const rapidjson::Value& mind = mindItr->value;
						if ( mind.IsInt() ) {
							videoBuilder.setMinDuration( mind.GetInt() );
						}
					}
					// maxDuration
					auto maxdItr = video.FindMember("maxduration");
					if ( maxdItr != video.MemberEnd() ) {
						const rapidjson::Value& maxd = maxdItr->value;
						if ( maxd.IsInt() ) {
							videoBuilder.setMaxDuration( maxd.GetInt() );
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
					auto dealItr = pmp.FindMember("deals");
					if ( dealItr != pmp.MemberEnd() ) {
						const rapidjson::Value& deals = dealItr->value;
						if ( deals.IsArray() && deals.Size() > 0 ) {
							const rapidjson::Value& deal = deals[0];
							// id
							auto dealidItr = deal.FindMember("id");
							if ( dealidItr != deal.MemberEnd() ) {
								const rapidjson::Value& dealid = dealidItr->value;
								if ( dealid.IsString() ) {
									pmpBuilder.setId( dealid.GetString() );
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
		AdsOs os = param.device().os();
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
	}

	// app
	auto appBuilder = param.app().getBuilder();
	auto appItr = doc.FindMember("app");
	if ( appItr != doc.MemberEnd() ) {
		const rapidjson::Value& app = appItr->value;
		// name
		auto nameItr = app.FindMember("name");
		if ( nameItr != app.MemberEnd() ) {
			const rapidjson::Value& name = nameItr->value;
			if ( name.IsString() ) {
				appBuilder.setName( name.GetString() );
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
			const rapidjson::Value& id = idItr->value;
			if ( id.IsString() ) {
				userBuilder.setExchangeId( id.GetString() );
			}
		}
	}

	return true;
}

static void packBiddingFailure(AdsBiddingParam& param, AdsHttpResponse *response)
{
	rapidjson::Document doc;
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value root(rapidjson::kObjectType);

	root.AddMember("id", rapidjson::StringRef(ads_str_data(&bid->id)), allocator); // BidRequest的唯一标识
	root.AddMember("nbr", ADVIEW_NBR_USER_UNMATCH, allocator);
	
	ads_json_to_str(root, &request->data_out);
	return ADS_OK;
}

void AdsAdviewExchange::packBiddingResponse(AdsBiddingParam& param,
	AdsAdvertise *ad, AdsHttpResponse *response)
{
	if ( ad == NULL ) {
		packBiddingFailure(param, response);
		return ;
	}



}

int AdsAdviewExchange::decryptWinPrice(const string& str)
{

	return 0;	
}