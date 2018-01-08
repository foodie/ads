#ifndef _ADS_CONTROLLER_H
#define _ADS_CONTROLLER_H

#include <string>
#include <functional>
#include <unordered_map>

using std::unordered_map;
using std::function;
using std::bind;

typedef struct _thread_data_t thread_data_t;

typedef thread_data_t AdsThreadData;

class AdsController
{
public:
	typedef function<int (AdsThreadData*)> AdsAction;

	AdsController() : actions() {}
	virtual ~AdsController();

	virtual int run(AdsThreadData*) = 0;

protected:

	void setAction(const string& n, AdsAction a)
	{ actions[n] = a; }

private:

	unordered_map<string, AdsAction> actions;
};

#endif
/* vim: set ts=4 sw=4 noet: */
