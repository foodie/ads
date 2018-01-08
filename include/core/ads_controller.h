#ifndef _ADS_CONTROLLER_H
#define _ADS_CONTROLLER_H

#include <string>
#include <functional>
#include <unordered_map>

using std::unordered_map;
using std::function;
using std::bind;

typedef struct _thread_data_t AdsThreadData;

class AdsController
{
public:
	//typedef function<int (AdsThreadData*)> AdsAction;

	AdsController() {}
	virtual ~AdsController() {}

	virtual int perform(AdsThreadData*) = 0;

// static //
public:
	static bool init();
	static void close();
	static AdsController* getController(const string &name);
private:
	static unordered_map<string, AdsController*> *map;
};

unordered_map<string, AdsController*>* AdsController::map = NULL;


#endif
/* vim: set ts=4 sw=4 noet: */
