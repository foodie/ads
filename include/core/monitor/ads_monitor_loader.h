#ifndef _ADS_MONITOR_LOADER_H
#define _ADS_MONITOR_LOADER_H


class AdsMonitorLoader
{
public:
	virtual void load() = 0;
};

// log
class AdsMonitorLogLoader : public AdsMonitorLoader
{
public:
	virtual void load() override
	{
		
	}
};

#endif
/* vim: set ts=4 sw=4 noet: */
