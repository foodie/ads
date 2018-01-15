#ifndef _ADS_BIDDING_PARAM_H
#define _ADS_BIDDING_PARAM_H

#include "core/bidding/ads_bidding.h"
#include "core/bidding/ads_bidding_types.h"

class AdsBiddingParam
{
public:
	enum class BiddingType { RTB = 1, PDB };

	void setExchangeId(int id) { exchange_id = id; }
	int getExchangeId() const { return exchange_id; }

	void setBiddingId(const string& id) { bidding_id = id; }
	const string& getBiddingId() const { return bidding_id; }

	void setBiddingType(BiddingType type) { bidding_type = type; }
	BiddingType getBiddingType() const { return bidding_type; }


	AdsBiddingImpression& getImpression() { return impression; }
	AdsBiddingDevice& getDevice() { return device; }
	AdsBiddingApp& getApp() { return app; }
	AdsBiddingUser& getUser() { return user; }

private:
	int 		exchange_id;
	string 		bidding_id;
	BiddingType bidding_type;

	AdsBiddingImpression 	impression;
	AdsBiddingDevice 		device;
	AdsBiddingApp 			app;
	AdsBiddingUser 			user;
};

#endif
/* vim: set ts=4 sw=4 noet: */
