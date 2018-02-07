#ifndef _ADS_API_CONTROLLER_H
#define _ADS_API_CONTROLLER_H

#include "plugins/controller/ads_controller.h"

class AdsApiController : public AdsController
{
public:

	virtual int process(AdsThreadData* pdata) override;

};

#endif
/* vim: set ts=4 sw=4 noet: */
