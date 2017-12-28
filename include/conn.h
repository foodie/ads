#ifndef  __CONN_H__
#define  __CONN_H__

#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "log.h"
#include "phashmap.h"
#include "cluster.h"

#define MAX_SVR_NUM			4096
#define MAX_PATH_LEN		512
#define MAX_ADDR_LEN		128
#define MAX_CLUTER_ADDR_LEN 1024

enum {
    FIXED = 0,
    ZOOKEEPER = 1
};

enum {
    SHORT_CONN = 0,
    LONG_CONN = 1
};

typedef struct _serv_addr_t
{
	char ip[MAX_ADDR_LEN];  //服务器地址
    uint32_t ip_int;        //IP数字地址
	uint32_t port;          //服务器端口
} serv_addr_t;


typedef struct _pool_conf_t pool_conf_t;
typedef struct _conn_serv_t conn_serv_t;

namespace conn
{

class Strategy;
class StrategyExt;
class StrategyDefault;
class HealthyChecker;
class CheckerDefault;
class ConnectManager;
class Server;
class Connection;

}

typedef struct _serv_info_t
{
    pthread_mutex_t zk_mutex;
    char zk_server[MAX_CLUTER_ADDR_LEN];
    char zk_node[MAX_PATH_LEN];
    conn::ConnectManager *mgr;
    serv_addr_t serv_addrs[MAX_SVR_NUM];
    uint32_t serv_num;
    uint32_t timeout;
    uint32_t capacity;
} serv_info_t;

typedef struct _pool_conf_t
{
    uint32_t type;
    serv_addr_t serv_addrs[MAX_SVR_NUM];
    uint32_t serv_num;
    uint32_t retry;
    uint32_t second;
    uint32_t timeout;
    uint32_t capacity;
    uint32_t conn_type;
    char zk_server[MAX_CLUTER_ADDR_LEN];
    char zk_node[MAX_PATH_LEN];
} pool_conf_t;

//获取配置
int pool_conf_init(const char *fpath, const char *fname, 
        pool_conf_t *conf, const char *strName = "pool");

class ConnPool
{
protected:
    conn::ConnectManager    *_mgr;
    conn::Strategy          *_stt;
    conn::HealthyChecker    *_checker;
    conn::StrategyExt		*_def_stt;
    conn::CheckerDefault    *_def_checker;
    lib::phashmap<int, conn::Connection *> _sockmap;

    uint32_t _type;
    serv_info_t _serv_info;

public:
    //初始化函数, 从一个配置初始化ConnPool, 不能重复init
    int init(pool_conf_t *conf);

    //销毁函数
    int destroy();

    /**
     * @brief 初始化函数, 用server列表初始化ConnectManager, 不能重复init
     * @param [in] svrs   : Server** server指针数组, 用户自己管理内存, 
     *                         ConnectManager内部会拷贝一份
     * @param [in] svrnum   : int server数量
     * @param [in] shortConnection   : bool 是否短连接
    **/
    int init(uint32_t type, conn::Server **svrs, uint32_t svr_num, 
            uint32_t second, uint32_t conn_type);

    /**
     * @brief 设置选择Server的策略
     *             初始化时操作, 非线程安全
    **/
    int setStrategy(conn::Strategy *stt);

    /**
     * @brief 设置健康检查策略
     *             初始化时操作, 非线程安全
    **/
    int setHealthyChecker(conn::HealthyChecker *hchk);

    /**
     * @brief 获取一个可用的连接
     * @param [in] key   : int 参考的选取参数
     * @param [in] waittime   : int 如果没有可用连接, 则等待多少毫秒
     * @param [in] *err : int * 如果FetchSocket失败返回 错误号,包括
     *          conn::REQ_SVR_FULL,
     *          conn::REQ_CONN_FAILED,
     *          conn::REQ_SVR_DISABLE
     * @return  成功返回的sock 
     * -1 失败 >0成功建立连接的sock
    **/
    int FetchSocket(int key = -1, int waittime = 0, int *err = NULL);

    /**
     * @brief 归还一个连接
     * @param [in] sock   : int
     * @param [in] errclose   : bool 是否出错了
    **/
    int FreeSocket(int sock, bool errclose);

    /**
     * @brief 根据sock获取对应的Connection
     *             便于对Connection的直接操作
    **/
    conn::Connection *getConnectionBySock(int sock);

    /**
     * @brief 获取底层的ConnectManager
     *             ConnPool是对ConnectManager的包装
     *             此接口便于用户的直接操作
    **/
    conn::ConnectManager *getManager();
    ConnPool();
    ~ConnPool();
};

#endif
