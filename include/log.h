#ifndef __LOG_H__
#define __LOG_H__

#include <elf.h>

#define LEVEL_NONE      0
#define LEVEL_FATAL     0x01    /**<   fatal errors */
#define LEVEL_WARN      0x02    /**<   exceptional events */
#define LEVEL_CRIT      0x03    /**<   critical events */
#define LEVEL_NOTICE    0x04    /**<   informational notices */
#define LEVEL_OTHER     0x05    /**<   other notices */
#define LEVEL_INFO      0x06    /**<   init server status */
#define LEVEL_TRACE     0x08    /**<   program tracing */
#define LEVEL_CONFIG    0x09    /**<   config information */
#define LEVEL_TUNING    0x0C    /**<   for online truning */
#define LEVEL_DEBUG     0x10    /**<   full debugging */
#define LEVEL_ALL       0xff    /**<   everything     */

#define PATH_SIZE           256
#define MAX_IP_SIZE         20
#define NOTICE_INFO_MAXLEN  10240

//一级日志信息保存结构
typedef struct _notice_info_t
{
    uint64_t logid;
    uint32_t cur_len;
    char reqip[MAX_IP_SIZE];
    char notice_str[NOTICE_INFO_MAXLEN]; /**< 日志保存串    */
} notice_info_t, *pnotice_info_t;

/**
 * @brief 初始化函数  打开日志。使用配置文件进行初始化。
 *
 * @param [in] fpath   : const char* 配置文件路径
 * @param [in] fname   : const char* 配置文件名
 * @return  int 0为成功, 其它失败
**/
int log_init(const char *fpath, const char *fname);

/**
 * @brief 线程中初始化函数时调用, 用于线程打印日志
 *       进入线程后调用
 * @param[in] thread_name 线程的信息
 * @return
 *        0表示成功
 *        -1设置日志失败
 */
int log_initthread(const char* thread_name);

/**
 * @brief 打印日志
 */
void log_write(int event, const char *fmt, ...);

/**
 * @brief 程序结束时调用, 用来关闭日志功能
 */
void log_close();


/**
 * @brief 程序结束时调用, 用来关闭日志功能
 */
void log_closethread();


/**
 * @brief 设置线程notice日志栈信息, 最大长度为 NOTICE_INFO_MAXLEN
 * @param[in] key key info of key:value
 * @param[in] valuefmt format of info
 * @return
 *      length of truely added info
 */
uint32_t log_pushnotice(const char* key, const char* valuefmt, ...);


/**
 * @brief set logid
 * @param logid logid of network
 * @return
 *        logid that set
 */
uint64_t log_setlogid(uint64_t logid);
uint64_t log_getlogid();

/**
 * @brief set reqip
 * @param reqip of network
 * @return
 *        reqip that set
 */
uint32_t log_setreqip(uint32_t reqip);
char    *log_getreqip();


/**
 * @brief 弹出push进入的信息
 * @return
 *        string of user puted
 * @comment for log_use
 */
char* log_popnotice();


/**
 * @brief 清空线程日志相关数据:在每次打NOTICE后, 库会自己调用
 * @return
 *      0表示成功
 *        其它表示失败
 * @comment for log_use
 **/
int log_clearnotice();


/**
 * @brief 得到线程的处理时间, 用于在LOG_DEBUG中打印距离上次打印的时间
 * @return
 *      返回自从上次调用以来执行的时间（微秒)
 * @comment for log_use
 */
unsigned int log_getussecond();

/**
 * @brief 内部cpp使用, 外部禁用
 */
int notice_specific_init();

/**
 * @brief 内部cpp使用, 外部禁用
 */
void log_keyonce();


#define FATAL(fmt, arg...) do { \
    log_write(LEVEL_FATAL, "%016lX\01[%s:%d][%s]\01" fmt, \
        log_getlogid(), __FILE__, __LINE__, \
        log_getreqip(), ## arg); \
} while (0)

#define WARN(fmt, arg...)  \
    log_write(LEVEL_WARN, "%016lX\01[%s:%d][%s]\01" fmt, \
        log_getlogid(), __FILE__, __LINE__, \
        log_getreqip(), ## arg)

#define CRIT(fmt, arg...)  \
    log_write(LEVEL_CRIT, "%016lX\01" fmt, \
        log_getlogid(), ## arg)

#define NOTICE(fmt, arg...) do { \
    log_write(LEVEL_NOTICE, "%016lX\01%s" fmt, \
        log_getlogid(), log_popnotice(), ## arg); \
    log_clearnotice(); \
} while (0)

#define OTHER(fmt, arg...)  \
    log_write(LEVEL_OTHER, "%016lX\01" fmt, \
        log_getlogid(), ## arg)

#define INFO(fmt, arg...)  \
    log_write(LEVEL_INFO, fmt, ## arg)

#define TRACE(fmt, arg...)  \
    log_write(LEVEL_TRACE, fmt, ## arg)

#define CONFIG(fmt, arg...)  \
    log_write(LEVEL_CONFIG, fmt, ## arg)

#define TUNING(fmt, arg...)  \
    log_write(LEVEL_TUNING, "%016lX\01" fmt, \
        log_getlogid(), ## arg)

#define DEBUG(fmt, arg...) \
    log_write(LEVEL_DEBUG, "%016lX\01[%s:%d]\01" fmt, \
        log_getlogid(), __FILE__, __LINE__, \
        ## arg)

#endif

/* vim: set ts=4 sw=4 tw=0 noet: */
