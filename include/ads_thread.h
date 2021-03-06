#ifndef _ADS_THREAD_H
#define _ADS_THREAD_H

#include "ads_def.h"
#include "ads_request.h"

class FCGX_Request;

typedef struct _thread_data_t
{	
	FCGX_Request 		*fcgx_request;

	AdsHttpRequest		*request;
	AdsHttpResponse		*response;

} thread_data_t;

typedef struct _thread_data_t AdsThreadData;

int thread_data_init(thread_data_t *p_thd_data);
int thread_data_renew(thread_data_t *p_thd_data);


#endif
/* vim: set ts=4 sw=4 noet: */
