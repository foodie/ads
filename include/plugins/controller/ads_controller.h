#ifndef _ADS_CONTROLLER_H
#define _ADS_CONTROLLER_H

#include "ads_thread.h"

typedef struct _thread_data_t AdsThreadData;

class AdsController
{
public:

	virtual int process(AdsThreadData*) = 0;

};

#endif
/* vim: set ts=4 sw=4 noet: */
