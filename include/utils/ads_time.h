#ifndef _ADS_TIME_H
#define _ADS_TIME_H

#include "time.h"
#include <string>

using std::string;

time_t ads_nowtime();

time_t ads_today();

time_t ads_first_day_of_week();

time_t ads_first_day_of_month();

string date(time_t ts);

#endif
/* vim: set ts=4 sw=4 noet: */
