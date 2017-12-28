#ifndef _ADS_THREAD_H
#define _ADS_THREAD_H

#include <fcgi_stdio.h>

#include "ads_def.h"
#include "ads_request.h"

typedef struct _thread_data_t
{	
	FCGX_Request 			*fcgx_request;

} thread_data_t;

int thread_data_init(thread_data_t *p_thd_data);
int thread_data_renew(thread_data_t *p_thd_data);


#endif
/* vim: set ts=4 sw=4 noet: */
