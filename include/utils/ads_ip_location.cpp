#include "utils/ads_ip_location.h"

#include "log.h"
#include "ads_conf.h"

/***************static****************/

bool AdsIpLocation::isIpString(const char *ip) 
{
	int len = strlen(ip);
	// 至少7个字符串0.0.0.0
	// 最多15个字符串255.255.255.255
	// 开头不能为'.'
	if (len < 7 || len > 15 || *ip == '.') {
		return false;
	}
	short p = 0; // 段数
	short s = 0; // 每段的数值
	char c;
	for (int i = 0; i < len; i++) {
		c = ip[i];
		if (c == '.') {
			if (s > 255) {
				return false;
			}
			s = 0;
			p++;
		} else {
			if (c < '0' || c > '9') {
				return false;
			}
			s = s * 10 + c - '0';
		}
	}
	// 3段
	if (p != 3) {
		return false;
	}
	return s <= 255;
}

string AdsIpLocation::long2Ip(long ipval)
{
	char ipstr[16];
	snprintf(ipstr, 16, "%d.%d.%d.%d", 
					(int) ((ipval & 0xFF000000) >> 24),  
					(int) ((ipval & 0x00FF0000) >> 16), 
					(int) ((ipval & 0x0000FF00) >> 8), 
					(int) (ipval & 0x000000FF));
	return ipstr;
}

long AdsIpLocation::ip2Long(const char *ip) 
{
	if ( !isIpString(ip) ) {
		return 0;
	}
	uint32_t l = 0;
	for (int i = 0; i < 4; i++) {
		char v = (char) atoi(ip);
		memcpy((char*)&l + 3 - i, &v, 1);
		ip = strchr(ip, '.') + 1;
	}
	return l;
}

/***************static****************/


uint32_t AdsIpLocation::_search_ip(uint32_t ip, uint32_t start=0, uint32_t end=0) const {
	if (0 == start) {
		start = _index_start;
	}
	if (0 == end) {
		end = _index_end;
	}
	
	uint32_t mid = end;
	do {
		uint32_t mid_ip = this->_get_value_4(mid);
			
		if (mid_ip > ip) {
			end = mid;
		} else {
			start = mid;
		}
		mid = (end - start) / WRY_INDEX_LEN / 2 * WRY_INDEX_LEN + start;
	} while(start < mid);
	
	return mid;
}

bool AdsIpLocation::open(const char *fpath)
{
	FILE *file = fopen(fpath, "r");  
	if (file == NULL) {
		FATAL("open address index file failed");
		return false;  
	}  
	fread(_index_data, 1, WRY_FILE_LEN, file);
		
	this->_index_start = this->_get_value_4(0);
	this->_index_end = this->_get_value_4(4);
		
	return true;
}

bool AdsIpLocation::init()
{
	bool ret = open( g_conf->ipdat_fpath );
	if ( !ret ) {
		WARN("[IpLocation] open dat failed");
        return false;
	}

	INFO("[IpLocation] IpLcation init success");
	return true;
}

int AdsIpLocation::getLocationId(const char *ip)
{
	if ( !isIpString(ip) ) {
		return -1;
	}
	uint32_t ipl = ip2Long(ip);
	uint32_t offset = _search_ip(ipl);
	uint32_t idl = this->_get_value_4(offset + WRY_IP_LEN * 2);
	return idl;
}
