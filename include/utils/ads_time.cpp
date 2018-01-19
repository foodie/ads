#include "utils/ads_time.h"

time_t ads_nowtime()
{
	return time(NULL);
}

time_t ads_today()
{
	time_t nowtime = ads_nowtime();
	time_t today = nowtime - (nowtime + 8*3600) % (24*3600);
	return today;
}

time_t ads_first_day_of_week()
{
	time_t today = ads_today();
	struct tm *t = localtime(&today);
	int week = t->tm_wday == 0 ? 7 : t->tm_wday;
	time_t weekday = today - (week-1) * (24*3600);	
	return weekday;
}

time_t ads_first_day_of_month()
{
	time_t today = ads_today();
	struct tm *t = localtime(&today);
	int day = t->tm_mday;
	time_t monday = today - (day-1) * (24*3600);
	return monday;
}

