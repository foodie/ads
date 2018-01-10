#ifndef _ADS_DATA_H
#define _ADS_DATA_H

#include <string>

#include "utils/ads_ip_location.h"

struct g_data_t
{
	AdsIpLocation *iploc;
};

extern g_data_t g_data;

int g_data_init();

int g_data_renew();


#endif
/* vim: set ts=4 sw=4 noet: */
