#ifndef _ADS_IP_LOCATION_H
#define _ADS_IP_LOCATION_H

class AdsIpLocation
{
public:
	static bool isIpString(const char *ip) {
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

	static string long2Ip(long ipval)
	{
		char ipstr[16];
		snprintf(ip, 16, "%d.%d.%d.%d", 
						(int) ((ipstr & 0xFF000000) >> 24),  
						(int) ((ipstr & 0x00FF0000) >> 16), 
						(int) ((ipstr & 0x0000FF00) >> 8), 
						(int) (ipstr & 0x000000FF));
		return ipstr;
	}
	static long ip2Long(const char *ip) {
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

private:
	

};

#endif
/* vim: set ts=4 sw=4 noet: */
