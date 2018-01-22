#include "plugins/controller/ads_monitor_controller.h"

#include "core/monitor/ads_monitor_parse.h"
#include "core/monitor/ads_monitor_service.h"

int AdsMonitorController::process(AdsThreadData* p_thd_data)
{
	bool ret;
	AdsMonitorParam param;

	ret = AdsMonitorParse::parseMonitorRequest(p_thd_data->request, &param);
	if ( !ret ) {
		WARN("[Monitor] parse request failed");
		return ADS_HTTP_BAD_REQUEST;
	}

	AdsMonitorService& serv = getMonitorService();
	serv.update(&param);

	return ADS_HTTP_OK;
}
