#ifndef _ADS_BIDDING_TYPES_H
#define _ADS_BIDDING_TYPES_H

#include "core/ads_core.h"

/**
 * 使用匿名内部类Builder进行读写分离
 */

/**
 * @brief      Class for ads bidding banner.
 */
class AdsBiddingBanner
{
	friend class Builder;
private:
	class Builder
	{
	public:
		Builder(AdsBiddingBanner& banner) : dst(banner) {}
		void setWidth(int width) { dst._width = width; }
		void setHeight(int height) { dst._height = height; }
		void setPos(int pos) { dst._pos = pos; }
	private:
		AdsBiddingBanner& dst;
	};

public:
	AdsBiddingBanner() : _builder(*this),_width(0),_height(0),_pos(0) {}
	Builder& getBuilder() { return _builder; }

	int width() const { return _width; }
	int height() const { return _height; }
	int pos() const { return _pos; }
private:
	Builder _builder;
	int _width;
	int _height;
	int _pos;
};

/**
 * @brief      Class for ads bidding video.
 */
class AdsBiddingVideo
{
	friend class Builder;
private:
	class Builder
	{
	public:
		Builder(AdsBiddingVideo& video) : dst(video) {}
		void setWidth(int width) { dst._width = width; }
		void setHeight(int height) { dst._height = height; }
		void setMinDuration(int duration) { dst._minduration = duration; }
		void setMaxDuration(int duration) { dst._maxduration = duration; }
	private:
		AdsBiddingVideo& dst;
	};

public:
	AdsBiddingVideo() : _builder(*this),_width(0),_height(0),_minduration(0),_maxduration(0) {}
	Builder& getBuilder() { return _builder; }

	int width() const { return _width; }
	int height() const { return _height; }
	int minDuration() const { return _minduration; }
	int maxDuration() const { return _maxduration; }
private:
	Builder _builder;
	int _width;
	int _height;
	int _minduration;
	int _maxduration;
};

/**
 * @brief      Class for ads bidding native.
 */
class AdsBiddingNative
{
};

/**
 * @brief      Class for ads bidding pmp.
 */
class AdsBiddingPmp
{
	friend class Builder;
private:
	class Builder
	{
	public:
		Builder(AdsBiddingPmp& pmp) : dst(pmp) {}
		void setId(const string& id) { dst._id = id; }
	private:
		AdsBiddingPmp& dst;
	};

public:
	AdsBiddingPmp() : _builder(*this),_id() {}
	Builder& getBuilder() { return _builder; }

	const string& id() const { return _id; }
private:
	Builder _builder;
	string _id;
};

/**
 * @brief      Class for ads bidding impression.
 */
class AdsBiddingImpression
{
	friend class Builder;
private:
	class Builder
	{
	public:
		Builder(AdsBiddingImpression& imp) : dst(imp) {}

		void setId(const string& id) { dst._id = id; }
		void setZoneId(const string& id) { dst._zoneid = id; }
		
		void setType(AdsImpressionType type) { dst._type = type; }
		void setSettlement(AdsBiddingSettlement settlement) { dst._settlement = settlement; }
		void setBidFloor(int floor) { dst._bidfloor = floor; }

		void setIsPmp(bool flag) { dst._ispmp = flag; }
	private:
		AdsBiddingImpression& dst;
	};

public:
	AdsBiddingImpression() 
		: _builder(*this),
		  _id(),_zoneid(),
		  _type(AdsImpressionType::BANNER),_settlement(AdsBiddingSettlement::CPM),
		  _bidfloor(0), 
		  _banner(),_video(),_native(),
		  _ispmp(false),_pmp()
	{}
	Builder& getBuilder() { return _builder; }

	const string& id() const { return _id; }
	const string& zoneId() const { return _zoneid; }

	AdsImpressionType type() const { return _type; }
	AdsBiddingSettlement settlement() const { return _settlement; }
	int bidFloor() const { return _bidfloor; }

	const AdsBiddingBanner& banner() const { return _banner; }
	const AdsBiddingVideo& video() const { return _video; }
	const AdsBiddingNative& native() const { return _native; }

	bool isPmp() const { return _ispmp; }
	const AdsBiddingPmp& pmp() const { return _pmp; }
private:
	Builder _builder;

	string _id;
	string _zoneid; // 广告位
	
	AdsImpressionType _type; // 展示类型
	AdsBiddingSettlement _settlement; // 计费类型
	int _bidfloor; // 底价

	AdsBiddingBanner _banner;
	AdsBiddingVideo  _video;
	AdsBiddingNative _native;

	bool _ispmp;
	AdsBiddingPmp _pmp;
};

/**
 * @brief      Class for ads bidding device.
 */
class AdsBiddingDevice
{
	friend string getDeviceId(const AdsBiddingDevice &device)
	{
		if ( device.os() == AdsOs::IOS ) {
			return device.idfa();
		}
		if ( device.os() == AdsOs::ANDROID ) {
			return device.imei();
		}
		return device.mac();
	}

	friend class Builder;
private:
	class Builder
	{
	public:
		Builder(AdsBiddingDevice& dev) : dst(dev) {}

		void setType(AdsDeviceType type) { dst._type = type; }
		void setOs(AdsOs os) { dst._os = os; }
		void setCarrier(AdsCarrier carrier) { dst._carrier = carrier; }
		void setConnectionType(AdsConnectionType type) { dst._connectiontype = type; }

		void setIp(const string& ip) { dst._ip = ip; }
		void setIdfa(const string& idfa) { dst._idfa = idfa; }
		void setImei(const string& imei) { dst._imei = imei; }
		void setMac(const string& mac) { dst._mac = mac; }
		void setMac1(const string& mac1) { dst._mac1 = mac1; }

	private:
		AdsBiddingDevice& dst;
	};

public:
	AdsBiddingDevice() 
		: _builder(*this),
		  _type(AdsDeviceType::UNKNOW),_os(AdsOs::UNKNOW),
		  _carrier(AdsCarrier::UNKNOW),_connectiontype(AdsConnectionType::UNKNOW),
		  _ip(),_idfa(),_imei(),_mac(),_mac1()
	{}
	Builder& getBuilder() { return _builder; }

	AdsDeviceType type() const { return _type; }
	AdsOs os() const { return _os; }
	AdsCarrier carrier() const { return _carrier; }
	AdsConnectionType connectionType() const { return _connectiontype; }

	const string& ip() const { return _ip; }
	const string& idfa() const { return _idfa; }
	const string& imei() const { return _imei; }
	const string& mac() const { return _mac; }
	const string& mac1() const { return _mac1; }

private:
	Builder _builder;

	AdsDeviceType 		_type;
	AdsOs 				_os;
	AdsCarrier 			_carrier;
	AdsConnectionType 	_connectiontype;

	string _ip;
	string _idfa;	// 原值
	string _imei;	// 大写,md5
	string _mac;	// 无分隔符,md5
	string _mac1;	// 有分隔符,md5
};

/**
 * @brief      Class for ads bidding application.
 */
class AdsBiddingApp
{
	friend class Builder;
private:
	class Builder
	{
	public:
		Builder(AdsBiddingApp& app) : dst(app) {}
		void setName(const string& name) { dst._name = name; }
	private:
		AdsBiddingApp& dst;
	};

public:
	AdsBiddingApp() : _builder(*this),_name() {}
	Builder& getBuilder() { return _builder; }
	const string& name() const { return _name; }
private:
	Builder _builder;
	string _name;
};

/**
 * @brief      Class for ads bidding user.
 */
class AdsBiddingUser
{
	friend class Builder;
private:
	class Builder
	{
	public:
		Builder(AdsBiddingUser& user) : dst(user) {}
		void setExchangeId(const string& id) { dst._exchangeid = id; }
	private:
		AdsBiddingUser& dst;
	};

public:
	AdsBiddingUser() : _builder(*this),_exchangeid() {}
	Builder& getBuilder() { return _builder; }
	const string& exchangeId() const { return _exchangeid; }
private:
	Builder _builder;
	string _exchangeid;	// 平台方id
};


#endif
/* vim: set ts=4 sw=4 noet: */
