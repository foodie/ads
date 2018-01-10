#ifndef _ADS_TIME_H
#define _ADS_TIME_H

#include <sys/time.h>

time_t ads_nowtime();

time_t ads_today();

time_t ads_first_day_of_week();

time_t ads_first_day_of_month();

#endif
/* vim: set ts=4 sw=4 noet: */
