#include "ads_func.h"

#include "coding.h"
#include "log.h"
#include "ads_md5.h"
#include "ads_string.h"

std::string seconds_to_his(int seconds)
{
	int hour,minute,second;
	std::string his = "";
	// 计算时分秒
	hour = seconds / 3600;
	minute = seconds / 60 % 60;
	second = seconds % 60;
	// 组成字符串
	his += hour   < 10 ? "0" + ads_int_to_string(hour)   : ads_int_to_string(hour);
	his += ":";
	his += minute < 10 ? "0" + ads_int_to_string(minute) : ads_int_to_string(minute);
	his += ":";
	his += second < 10 ? "0" + ads_int_to_string(second) : ads_int_to_string(second);
	return his;
}

/**
 * @brief      device
 */

void ads_imei_md5(const char *imei, char *imei_md5)
{
	char buffer[40];
	size_t len = snprintf(buffer, sizeof(buffer), "%s", imei);
	if (len == 0) {
		return;
	}
	ads_char_toupper(buffer);
	ads_md5(buffer, len, imei_md5);
}

void ads_androidid_md5(const char *androidid, char *androidid_md5)
{
	char buffer[40];
	size_t len = snprintf(buffer, sizeof(buffer), "%s", androidid);
	if (len == 0) {
		return;
	}
	ads_char_toupper(buffer);
	ads_md5(buffer, len, androidid_md5);
}

void ads_mac_md5(const char *mac, char *mac_md5)
{
	char buffer[40];
	size_t len = snprintf(buffer, sizeof(buffer), "%s", mac);
	if (len == 0) {
		return;
	}
	ads_char_toupper(buffer);
	ads_md5(buffer, len, mac_md5);
}

void ads_mac_to_mac1(const char *mac, char *mac1)
{
	int i = 0;
	do {
		if (i == 2) {
			*mac1 = ':';
			mac1++;
			i = 0;
		}
		i++;
		*mac1 = *mac;
		mac1++;
		mac++;
	} while(*mac);
}

void ads_mac1_to_mac(const char *mac1, char *mac)
{
	do {
		if (*mac1 != ':') {
			*mac = *mac1;
		}
		mac1++;
		mac++;
	} while(*mac1);
}

uint32_t ads_ip_to_long(const char *ip)
{
	if (ads_strlen(ip) == 0) {
		return 0;
	}
	
	uint32_t l = 0;
	for (int i = 0; i < 4; i++) {
		char v = (char) atoi(ip);
		ads_memcpy((char*)&l + 3 - i, &v, 1);
		ip = strchr(ip, '.') + 1;
	}
	return l;
}

void ads_long_to_ip(uint32_t l, char *ip)
{
	snprintf(ip, 16, "%d.%d.%d.%d", 
					(int) ((l & 0xFF000000) >> 24),  
					(int) ((l & 0x00FF0000) >> 16), 
					(int) ((l & 0x0000FF00) >> 8), 
					(int)  (l & 0x000000FF));
}
