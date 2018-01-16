#include "core/advertise/ads_advertise_manager.h"


AdsAdvertiseManager* AdsAdvertiseManager::instance = new AdsAdvertiseManager(new AdsAdvertiseApiLoader);
