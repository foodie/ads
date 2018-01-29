#include "core/advertise/ads_advertise_loader.h"

#include "log.h"
#include "ads_conf.h"
#include "utils/ads_string.h"
#include "utils/ads_curl.h"
#include "utils/ads_json.h"

static void _split_vi(const string& str, vector<int>& vec, char sep);
static void _split_si(const string& str, unordered_set<int>& vec, char sep);

void AdsAdvertiseApiLoader::load(AdsAdvertiseCollection* collection)
{
	AdsCurl curl;
	curl.setRequestUrl( g_conf->ads.advertise_api );
	curl.setRequestMethod(ADS_HTTP_POST);
	if ( !curl.execute() ) {
		WARN("[Advertise] Query advertise data failed");
		return;
	}
	const string& body = curl.getResponseBody();

	rapidjson::Document doc;
	if (doc.Parse( body.c_str() ).HasParseError()) {
		WARN("[Advertise] advertise data parse failed");
		return;
	}

	// 素材
	auto materialItr = doc.FindMember("material");
	if ( materialItr != doc.MemberEnd() ) {
		const rapidjson::Value& materials = materialItr->value;
		if ( materials.IsArray() ) {
			for ( auto& material : materials.GetArray() ) {
				int id = material["id"].GetInt();
				AdsMaterialType type = AdsMaterialType( material["type"].GetInt() );
				AdsMaterial *mat = collection->addMaterial(id, type);

				if ( type == AdsMaterialType::IMAGE ) {
					AdsMaterialImage *img = mat->image();
					img->type 	= AdsMaterialImageType::IMAGE;
					img->width 	= material["width"].GetUint();
					img->height = material["height"].GetUint();
					img->ext 	= material["ext"].GetString();
					img->mime 	= material["mime"].GetString();
					img->source = material["url"].GetString();
				} else if ( type == AdsMaterialType::VIDEO ) {
					AdsMaterialVideo *video = mat->video();
					video->width 	= material["width"].GetUint();
					video->height 	= material["height"].GetUint();
					video->duration = material["duration"].GetUint();
					video->ext 		= material["ext"].GetString();
					video->mime 	= material["mime"].GetString();
					video->source 	= material["url"].GetString();
				} else if ( type == AdsMaterialType::NATIVE ) {
					
				}

			}
		}
	}

	// 活动
	auto campaignItr = doc.FindMember("campaign");
	if ( campaignItr != doc.MemberEnd() ) {
		const rapidjson::Value& campaigns = campaignItr->value;
		if ( campaigns.IsArray() ) {
			for ( auto& campaign : campaigns.GetArray() ) {
				int id = campaign["id"].GetInt();
				AdsCampaign *cap = collection->addCampaign(id);

				cap->budget 	= campaign["budget"].GetUint();
				cap->imp_limit 	= campaign["imp_limit"].GetUint();
				cap->clk_limit 	= campaign["clk_limit"].GetUint();

				cap->imp_freq 	= campaign["imp_freq"].GetUint();
				cap->clk_freq 	= campaign["clk_freq"].GetUint();

				cap->start_date = campaign["start_date"].GetUint64();
				cap->end_date 	= campaign["end_date"].GetUint64();
			}
		}
	}

	// 投放
	auto launchItr = doc.FindMember("launch");
	if ( launchItr != doc.MemberEnd() ) {
		const rapidjson::Value& launchs = launchItr->value;
		if ( launchs.IsArray() ) {
			for ( auto& launch : launchs.GetArray() ) {
				int id = launch["id"].GetInt();
				AdsLaunch *lau = collection->addLaunch(id);

				lau->settlement		= AdsBiddingSettlement( launch["charge_type"].GetInt() );
				lau->bidding_type 	= AdsBiddingType( launch["bidding_type"].GetInt() );
				lau->price 			= launch["price"].GetUint();
				lau->pmp_id 		= ads_int_to_string( launch["pdb_id"].GetInt() );

				lau->imp_freq 	= launch["imp_freq"].GetUint();
				lau->clk_freq 	= launch["clk_freq"].GetUint();

				lau->start_date = launch["start_date"].GetUint64();
				lau->end_date 	= launch["end_date"].GetUint64();

				lau->schedule 	= AdsAdvertiseSchedule( launch["schedule"].GetInt() );
			    for ( auto& b : launch["budget"].GetArray() ) {
			    	lau->budget.push_back( b.GetInt() );
			    }
				lau->speed 		= AdsAdvertiseSpeed( launch["speed"].GetInt() );

				lau->exchangeid	= launch["media_id"].GetInt();

				for ( auto& a : launch["area"].GetArray() ) {
					lau->area.emplace( a.GetInt() );
				}
				lau->os 		= AdsOsOrientation( launch["os"].GetInt() );
				lau->device 	= AdsDeviceOrientation( launch["device"].GetInt() );
				for ( auto& c : launch["connection_type"].GetArray() ) {
					lau->connection_type.emplace( c.GetInt() );
				}
				for ( auto& c : launch["carrier"].GetArray() ) {
					lau->carrier.emplace( c.GetInt() );
				}

				int campaign_id = launch["campaign_id"].GetInt();
				AdsCampaign *cap = collection->getCampaign(campaign_id);
				if ( cap == NULL ) {
					WARN("[Advertise] AdsCampaign is null : %d", campaign_id);
					continue;
				}
				lau->campaign = cap;
				cap->addLaunch( lau );
			}
		} 
	}

	// 创意
	auto creativeItr = doc.FindMember("creative");
	if ( creativeItr != doc.MemberEnd() ) {
		const rapidjson::Value& creatives = creativeItr->value;
		for ( auto& creative : creatives.GetArray() ) {
			int id = creative["id"].GetInt();
			AdsCreative *cre = collection->addCreative(id);

			cre->zone_id = creative["tagid"].GetString();

			cre->action = AdsClickAction( creative["ad_act"].GetInt() );
			cre->landing = creative["landing"].GetString();
			for ( auto& i : creative["imp_track"].GetArray() ) {
				cre->imp_track.push_back( i.GetString() );
			}
			for ( auto& c : creative["clk_track"].GetArray() ) {
				cre->clk_track.push_back( c.GetString() );
			}

			cre->effective 	= creative["effective"].GetInt();
			cre->start_date = creative["start_date"].GetUint64();
			cre->end_date 	= creative["end_date"].GetUint64();

			int launch_id = creative["launch_id"].GetInt();
			AdsLaunch *lau = collection->getLaunch(launch_id);
			if ( lau == NULL ) {
				WARN("[Advertise] AdsLaunch is null : %d", launch_id);
				continue;
			}
			cre->campaign 	= lau->campaign;
			cre->launch 	= lau;
			lau->addCreative(cre);

			int material_id = creative["material_id"].GetInt();
			AdsMaterial *mat = collection->getMaterial(material_id);
			cre->material = mat;
		}
	}

	//INFO("[Advertise] api load advertise success");
}

static void _split_vi(const string& str, vector<int>& vec, char sep)
{
	size_t p1=0, p2=0;
	while( p2 != string::npos ) {
	    p2 = str.find(sep, p1);
	    vec.push_back( ads_string_to_int(str.substr(p1, p2 - p1)) );
		p1 = p2 + 1;
	}
}

static void _split_si(const string& str, unordered_set<int>& s, char sep)
{
	size_t p1=0, p2=0;
	while( p2 != string::npos ) {
	    p2 = str.find(sep, p1);
	    s.emplace( ads_string_to_int(str.substr(p1, p2 - p1)) );
		p1 = p2 + 1;
	}
}