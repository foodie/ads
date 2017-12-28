#ifndef _SERV_H
#define _SERV_H

#include <elf.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "cap.h"

#define NAME_SIZE	4
#define FILE_SIZE   1024
#define DEVICE_SIZE 256
#define FILTER_SIZE 1024

#ifndef HAVE_HEAD
#define HAVE_HEAD
//网络交互统一的包头
static const unsigned int HEAD_MAGICNUM = 0xECD5BCBC;
typedef struct _head_t
{
    uint32_t    magic_num;	//魔数
    uint32_t    body_len;	//消息体字节长度
    uint64_t    log_id;		//日志唯一ID，标明一次检索
} head_t;
#endif

//设置服务器句柄
enum {
	LINGER_CLOSE = 0x1,
	NODELAY = 0x2,
    DEFER_ACCEPT = 0x4,
};

//设置服务器的长短连接
enum {
	SHORT_CONNECT = 0,	//短连接
	LONG_CONNECT		//长连接
};

enum {
	QUELE_LEN_PARAM,
	SOCKET_SIZE_PARAM,
	TIMEOUT_PARAM
};

typedef int (* callback_proc)();
typedef void (* callback_cap)(u_char*, const pcap_pkthdr*, const u_char*);
typedef int (* callback_writedone)(int write_result);
typedef int	(* handle_func_t)();

//服务的配置
typedef struct serv_conf_t
{
    //基本配置
	char	    name[NAME_SIZE];        //服务名称
    uint32_t    type;                   //服务类型, 0为网络 1为抓包 2为文件
	uint32_t	thread_num;             //线程数
	uint32_t	queue_size;				//队列缓冲长度
	uint32_t	socket_size;			//sock缓冲长度
    uint32_t    keepalive_timeout;      //保持连接超时

    //线程调度
    uint32_t    thread_sche;
    int         listen_prio;
    int         worker_prio;

    //网络模型
	uint32_t	port;                   //服务端口
    uint32_t    backlog;                //服务backlog
	uint32_t	connect_type;           //连接类型, 0短连接 1长连接
	uint32_t	read_buf_size;	    	//用户读缓冲区大小
	uint32_t	write_buf_size; 		//用户写缓冲区大小
	uint32_t	read_timeout;           //读超时
	uint32_t	write_timeout;          //写超时
	uint32_t	connect_timeout;		//连接超时

    //抓包模型
	char	    device[DEVICE_SIZE];    //抓包设备
	char	    filter[FILTER_SIZE];    //过滤规则
	char	    pcap_file[FILE_SIZE]; 	//抓包文件路径
	uint32_t	check_interval;		    //检测间隔
} serv_conf_t;

typedef struct _serv_pdata_t
{
	pthread_t pid;		//当前的线程号
	uint32_t id;		//当前线程id, 是启动的第几个id
	in_addr_t ip;		//当前连接句柄的ip
	int fd;				//当前的连接句柄

	void    *read_buf;		//读缓冲区
	void    *write_buf;	    //写缓冲区
	uint32_t read_buf_size;	//读缓冲区大小
	uint32_t write_buf_size;//写缓冲区大小
	void    *user_buf;		//用户缓冲区
	uint32_t user_size;	    //用户缓冲区大小
	void *parent;

	int self_type;

    long long sock_num_dec;
    iovec *iov;
    size_t iov_count;
} serv_pdata_t;

typedef struct _serv_t
{
	char     name[NAME_SIZE];   //服务名称
	uint32_t type;	            //服务类型
	uint32_t connect_type;	    //连接类型

	int      run;			    //服务器运行状态
	int      serv_sock;		    //服务器socket

	int      need_join;		    //销毁的时候是否需要join
	int      thread_num;	    //实际启动的线程数
	int      pthread_num;	    //启动线程数
	uint32_t pthread_size;	    //用户线程变量大小
	uint32_t read_buf_size; 	    //读缓冲区大小
	uint32_t write_buf_size;    	//写缓冲区大小

    //线程调度
    uint32_t thread_sche;
    int listen_prio;
    int worker_prio;

    //网络模型
	uint32_t port;      	    //监听端口
	uint32_t backlog;		    //blacklog
	int      socktype;          //sockopt参数
	int read_timeout;	        //读超时
	int write_timeout;	        //写超时
	int connect_timeout;        //连接超时
	//网络回调
	callback_proc cb_first;		//处理第一层的回调
	callback_proc cb_second;	//处理第二层的回调
	callback_proc cb_third;		//处理第三层的回调

    //抓包模型
    pcap_t *pd;                 //pcap描述符
    char ebuf[256];             //错误缓存
    char device[DEVICE_SIZE];   //抓包设备
    char filter[FILTER_SIZE];   //过滤规则
    char pcap_file[FILE_SIZE];//抓包文件路径
    uint32_t check_interval;    //检测间隔
    //抓包回调
    callback_cap cb_cap;

	//线程池
	void *pool;			    //内部参数, 如果有实体化
	int pool_data[256];	    //保留Pool的参数空间

	//线程数据
	serv_pdata_t *data;	    //线程使用的内存空间
	void **user_read_buf;	//如果用户指定内存空间, 则无需分配
	void **user_write_buf;	//用户指定内存空间, 无需分配

	//线程启动和销毁的回调
	handle_func_t p_start;
	handle_func_t p_end;
} serv_t;

//创建服务器
serv_t *serv_create();

//从配置加载的字段, 在serv_create之后马上调用
int serv_load(serv_t *serv, serv_conf_t *serv_conf);

//根据配置启动服务器
int serv_conf_init(const char *fpath, const char *fname, 
        serv_conf_t *serv_conf);

//启动服务器
int serv_run(serv_t *);

//设置每个线程启动时候的回调函数
int serv_set_thread_start(serv_t *serv, handle_func_t op);

//设置线程退出时候的处理函数
int serv_set_thread_stop(serv_t *serv, handle_func_t op);

//停止服务器
int serv_stop(serv_t *);

//join 服务器
int serv_join(serv_t *);

//detach 服务器句柄
int serv_detach(serv_t *);

//销毁服务器
int serv_destroy(serv_t *);

//设置默认的回调函数, 采用head作为协议头
int serv_set_callback(serv_t *serv, callback_proc cb_deal);

//设置朴素的回调函数, 不采用head
int serv_set_raw_callback(serv_t *serv, callback_proc cb_deal);

//设置默认+无需返回的回调函数
int serv_set_nores_callback(serv_t *serv, callback_proc cb_deal);

//设置默认+后续处理的回调函数
int serv_set_post_callback(serv_t *serv, callback_proc cb_deal, callback_proc cb_post);

//设置抓包回调函数
int serv_set_cap_callback(serv_t *serv, callback_cap cb_cap);


//设置读写缓冲区
int serv_set_buffer(serv_t *serv, void **read_buf, int read_buf_size, 
    void **write_buf, int write_buf_size);

//设置用户线程变量大小
int serv_set_user_data(serv_t *serv, void *, uint32_t pthread_size);
int serv_set_pthread_size(serv_t *serv, uint32_t pthread_size);

//设置启动的线程数
int serv_set_pthread_num(serv_t *serv, int pnum);

//设置读写, 连接超时, 单位毫秒
int serv_set_timeout(serv_t *serv, int connect_timeout, 
    int read_timeout, int write_timeout);

//设置服务器句柄参数
//设置SERV_DEFER_ACCEPT时，connnect_timeout作为超时时间
//注意，尽量不要使用SERV_LINGER_CLOSE状态，高并发时易出错
int serv_setoptsock(serv_t *serv, int type);

//设置服务名字
int serv_set_name(serv_t *serv, const char *name);
//获取服务名字
const char *serv_get_name(serv_t *serv);
//设置服务类型
int serv_set_type(serv_t *serv, uint32_t type);

//设置监听句柄
int serv_set_listen_fd(serv_t *serv, int listenfd);

//设置连接类型
int serv_set_connect_type(serv_t *serv, int type);
//设置监听端口
int serv_set_port(serv_t *serv, int port);
//获取服务器的监听端口
int serv_get_port(serv_t *serv);
//获取当前线程内部连接的socket句柄
int serv_get_socket();
int serv_bind(serv_t *);
//得到连接端ip
in_addr_t serv_get_ip();

//获取用户数据地址
void *serv_get_user_data();
//获取读缓冲区
void *serv_get_read_buf();
//获取写缓冲区
void *serv_get_write_buf();
//获取读缓冲区大小
uint32_t serv_get_read_buf_size();
//获取写缓冲区大小
uint32_t serv_get_write_buf_size();

//得到读超时, 单位毫秒
int serv_get_read_timeout();
//得到写超时, 单位毫秒
int serv_get_write_timeout();
//得到连接超时, 单位毫秒
int serv_get_connect_timeout();

//设置抓包设备
int serv_set_device(serv_t *serv, const char *device);
//设置过滤规则
int serv_set_filter(serv_t *serv, const char *filter);
//设置抓包设备
int serv_set_pcap_file(serv_t *serv, const char *pcap_file);
//设置检测间隔
int serv_set_check_interval(serv_t *serv, uint32_t check_interval);
//获取抓包计数
int serv_get_pcap_stats(serv_t *serv, u_int *recv, u_int *drop, u_int *ifdrop);

//获取启动的线程数目
int serv_get_thread_num(serv_t *serv);

//获取线程id
uint32_t serv_get_thread_id();

//获取服务器句柄
serv_t *serv_get_server();

//设置关闭本次交互的句柄
void serv_set_session_connect_type(int cotp);
//获取本次交互的长短连接属性
int serv_get_session_connect_type(serv_t *serv);

//获取队列大小
long long serv_get_usingqueue_size(serv_t *serv);
//获取socket大小
long long serv_get_usingsocket_size(serv_t *serv);

//获取iovec *的指针 
struct iovec **serv_get_write_iovec();
//获取iovce_count的指针, 用于存值或者取值 
size_t *serv_get_write_iovec_count();
int serv_get_listen_priority(serv_t *serv);
int serv_get_worker_priority(serv_t *serv);
bool serv_get_thread_sched(serv_t *serv);

#endif
