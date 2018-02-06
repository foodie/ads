#ifndef _ADS_BIDDING_PARAM_H
#define _ADS_BIDDING_PARAM_H

#include "core/ads_core.h"
#include "core/bidding/ads_bidding_types.h"

class AdsBiddingParam
{
	friend class Builder;
private:
	class Builder
	{
	public:
		Builder(AdsBiddingParam& param) : dst(param) {}
		void setExchangeId(int id) { dst._exchangeid = id; }
		void setBiddingId(const string& id) { dst._biddingid = id; }
	private:
		AdsBiddingParam& dst;	
	};

public:
	AdsBiddingParam() 
		: _builder(*this),
		  _exchangeid(0),_biddingid(),
		  _device(),_app(),_user()
	{}
	Builder& getBuilder() { return _builder; }

	int exchangeId() const { return _exchangeid; }
	const string& biddingId() const { return _biddingid; }

	//AdsBiddingImpression& impression() { return _impression; }
	AdsBiddingDevice& device() { return _device; }
	AdsBiddingApp& app() { return _app; }
	AdsBiddingUser& user() { return _user; }

	//const AdsBiddingImpression& impression() const { return _impression; }
	const AdsBiddingDevice& device() const { return _device; }
	const AdsBiddingApp& app() const { return _app; }
	const AdsBiddingUser& user() const { return _user; }

private:
	Builder _builder;

	int 			_exchangeid;  	// exchange id
	string 			_biddingid;		// 竞价请求id

	//AdsBiddingImpression 	_impression;
	AdsBiddingDevice 		_device;
	AdsBiddingApp 			_app;
	AdsBiddingUser 			_user;
};

#endif
/* vim: set ts=4 sw=4 noet: */
