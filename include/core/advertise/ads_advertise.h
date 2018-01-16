#ifndef _ADS_ADVERTISE_H
#define _ADS_ADVERTISE_H

enum class AdsAdvertiseSchedule
{
	AVERAGE = 2,
	CUSTOM,
};

enum class AdsAdvertiseSpeed
{
	AVERAGE = 1,
	FAST,
};

enum class AdsOsOrientation
{
	NONE = 0,
	IOS,
	ANDROID,
};

enum class AdsDeviceOrientation
{
	NONE = 0,
	MOBILE,
	PAD,
};

#endif
/* vim: set ts=4 sw=4 noet: */
