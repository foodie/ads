#ifndef _ADS_IP_LOCATION_H
#define _ADS_IP_LOCATION_H

#include <stdlib>
#include <stdio>
#include <string>
#include <string.h>
#include "core/ads_singleton.h"

using std::string;

static const unsigned int WRY_FILE_LEN 	= 6*1024*1024;
static const unsigned int WRY_IP_LEN 	= 4;
static const unsigned int WRY_ID_LEN 	= 4;
static const unsigned int WRY_INDEX_LEN = 12;

class AdsIpLocation : public AdsSingleton<AdsIpLocation>
{
	friend class AdsSingleton<AdsIpLocation>;
public:
	static bool isIpString(const char *ip);
	static string long2Ip(long ipval);
	static long ip2Long(const char *ip);

public:
	~AdsIpLocation() {}
	bool open(const char *fpath);
	int getLocationId(const char *ip);

private:
	AdsIpLocation() : _index_start(0),_index_end(0) {}

	char _index_data[WRY_FILE_LEN];
	uint32_t _index_start;	// 索引区首偏移
	uint32_t _index_end;	// 索引区尾偏移

	char _get_char(uint32_t offset) const {
		return _index_data[offset];
	}
	const char* _get_char_p(uint32_t offset) const {
		return _index_data + offset;
	}
	uint32_t _get_value_4(uint32_t offset) const {
		uint32_t v = 0;
		ads_memcpy((char*)&v, this->_get_char_p(offset), 4);
		return v;
	}

	uint32_t _search_ip(uint32_t ip, uint32_t start=0, uint32_t end=0) const;
};

static inline int getLocationId(const char *ip)
{
	return AdsIpLocation::getInstance().getLocationId(ip);
}

#endif
/* vim: set ts=4 sw=4 noet: */
