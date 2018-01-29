#include "plugins/crontab/ads_crontab.h"

#include <sys/time.h>
#include <signal.h>
#include "log.h"
#include "utils/ads_time.h"
#include "utils/ads_string.h"
#include "plugins/crontab/ads_crontab_task.h"

static void crontab_handler(int arg)
{
	AdsCrontab& crontab = AdsCrontab::getInstance();
	crontab.process();
}

AdsCrontab::AdsCrontab()
{

}

AdsCrontab::~AdsCrontab()
{
	for ( auto itr = _tasks.begin(); itr != _tasks.end(); itr++ ) {
		delete itr->second;
	}
	_tasks.clear();
}

bool AdsCrontab::init()
{
    // 设置定时器
	time_t nowtime = ads_nowtime();
    struct itimerval t;
    t.it_interval.tv_usec = 0;
    t.it_interval.tv_sec = 60;
    t.it_value.tv_usec = 0;
    t.it_value.tv_sec = 60 - nowtime % 60;
    if ( setitimer( ITIMER_REAL, &t, NULL ) ) {
        WARN("[Crontab] settimer failed");
        return false;
    }
    signal( SIGALRM, crontab_handler );

	INFO("[Crontab] Crontab plugins init success");
	return true;
}

void AdsCrontab::process()
{
    time_t nowtime = ads_nowtime();
    for ( auto& task : _tasks ) {
        if ( checkTime(task.first, nowtime) ) {
            AdsCrontabTask *tk = task.second;
            tk->run();
        }
    }
	//INFO("[Crontab] master process run success");
}

void AdsCrontab::add(const string& format, AdsCrontabTask *task)
{
    _tasks.push_back( _Tasker(format, task) );
}

static bool check_time_rule(const string& rule, int val)
{
    // *
    if ( rule == "*" ) {
        return true;
    }

    string tmp(rule);
    string valStr = ads_int_to_string(val);
    size_t p;

    // a
    if ( rule == valStr ) {
        return true;
    }
    // a,b,...
    p = rule.find(',');
    if ( p != string::npos ) {
        tmp.append(",");
        return tmp.find( valStr + ',' ) == string::npos;
    }
    // - / 
    int step = 1;
    p = rule.find('/');
    if ( p != string::npos ) {
        step = ads_string_to_int( rule.substr(p+1) );
        tmp = rule.substr(0, p);
    }
    p = tmp.find('-');
    if ( p != string::npos ) {
        int min = ads_string_to_int( tmp.substr(0, p) );
        int max = ads_string_to_int( tmp.substr(p+1) );
        if ( val < min || val > max ) {
            return false;
        }
    }
    if ( val % step == 0 ) {
        return true;
    }

    return false;
}

bool AdsCrontab::checkTime(const string& format, time_t ts)
{
    struct tm *t = localtime(&ts);

    size_t p1=0,p2=0;
    string tmp;
    
    // 分
    p2 = format.find(' ', p1);
    tmp = format.substr(p1, p2-p1);
    if ( !check_time_rule(tmp, t->tm_min) ) {
        return false;
    }

    // 时
    p1 = p2 + 1;
    p2 = format.find(' ', p1);
    tmp = format.substr(p1, p2-p1);
    if ( !check_time_rule(tmp, t->tm_hour) ) {
        return false;
    }

    // 日
    p1 = p2 + 1;
    p2 = format.find(' ', p1);
    tmp = format.substr(p1, p2-p1);
    if ( !check_time_rule(tmp, t->tm_mday) ) {
        return false;
    }

    // 月
    p1 = p2 + 1;
    p2 = format.find(' ', p1);
    tmp = format.substr(p1, p2-p1);
    if ( !check_time_rule(tmp, t->tm_mon+1) ) {
        return false;
    }

    // 周
    p1 = p2 + 1;
    p2 = format.find(' ', p1);
    tmp = format.substr(p1, p2-p1);
    if ( !check_time_rule(tmp, t->tm_wday+1) ) {
        return false;
    }

    return true;
}