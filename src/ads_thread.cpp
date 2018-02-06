#include "ads_thread.h"

#include "log.h"

int thread_data_init(thread_data_t *p_thd_data)
{
	p_thd_data->request = new AdsHttpRequest;
	p_thd_data->response = new AdsHttpResponse;

	return ADS_OK;
}

int thread_data_renew(thread_data_t *p_thd_data)
{
	p_thd_data->request->reset();
	p_thd_data->response->reset();
	return ADS_OK;
}
