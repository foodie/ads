#ifndef _RELOADER_H
#define _RELOADER_H
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <sys/stat.h>
#include <string.h>
#include <new>
#include "log.h"

// @brief   双 buffer 管理类
// @note    T 一定要实现 int load(const char* cp_dir, const char* cp_fname) 函数, 否则编译失败
// @note    如果有 init 需求, 可以在构造时, 提供一个 int T::init() 函数
// @note    以上两个函数在返回 < 0 时，表示失败
// @note    推荐设计：文件不存在或者内容为空，也认为是成功
template <class T>
class reloader_t
{
public:
    typedef T content_type;

    // @param   init_func，如果传入的话，reload会对new出来的对象调用 init_func
    reloader_t(const char* cp_path,
               const char* cp_fname,
               int (T::*init_func)() = NULL);
    ~reloader_t();

    // @return  -1, 文件不存在; 0, 不需要reload; 1, 可以reload
    int need_reload() const;

    // @brief   双 buffer 切换
    // @return  -1, reload失败; 1, reload成功
    int reload();

    // 取得当前可以使用的 buffer 的指针
    T* get_content();

    // 取得当前可以使用的 buffer 的指针
    const T* get_content() const;

    time_t get_file_modtime() const
    {
        return _tm;
    }

    time_t get_file_loadtime() const
    {
        return _last_update_tm;
    }

protected:
    time_t _tm;
    //运维需求
    time_t _last_update_tm; 
    uint8_t _using_no;

    // 两个buffer的指针
    T* _p_content[2];

    // 实际的buffer，避免reload时频繁的申请内存
    char _mem[2 * sizeof(T)];

    char _path[512];
    char _fname[512];

    // 个性 init 函数
    int (T::*_init_func)();

    //以下函数保证 T::load 的返回值必须为int
    typedef char small_t;
    struct big_t {
        char c[2];
    };

    static small_t _is_int(int);
    template <class V>
    static big_t _is_int(V);

    template <class RT, class CLA>
    static RT _get_load_ret_type(RT (CLA::*)(const char*, const char*));    
};

template <class T>
reloader_t<T>::reloader_t(const char* cp_path,
                          const char* cp_fname,
                          int (T::*init_func)()):
    _tm(0),
    _last_update_tm(0),
    _using_no(1),
    _init_func(init_func)
{
    // 静态检查 load 的返回值必须是 int
    static_assert(sizeof(small_t) == sizeof(_is_int(_get_load_ret_type(&T::load))), "error");

    if (cp_path == NULL) {
        cp_path = "./";
    }
    if (cp_fname == NULL) {
        cp_fname = "reload_file";
    }
    snprintf(_path, sizeof(_path), "%s", cp_path);
    snprintf(_fname, sizeof(_fname), "%s", cp_fname);

    _p_content[0] = NULL;
    _p_content[1] = NULL;

    memset(&_mem, 0, sizeof(_mem));
}

template <class T>
reloader_t<T>::~reloader_t()
{
    if (_p_content[0] != NULL)
    {
        //delete _p_content[0];
        _p_content[0]->~T();
        _p_content[0] = NULL;
    }

    if (_p_content[1] != NULL)
    {
        //delete _p_content[1];
        _p_content[1]->~T();
        _p_content[1] = NULL;
    }
}

template <class T>
int reloader_t<T>::need_reload() const
{
    char fpath[sizeof(_path) + sizeof(_fname) + 1] = "";
    snprintf(fpath, sizeof(fpath), "%s/%s", _path, _fname);

    struct stat curr_stat;

    // 记录当前状态文件状态
    if ((stat(fpath, &curr_stat) != 0) 
        || ((curr_stat.st_mode & S_IFREG) == 0))
    {
        return -1;
    }

    if (curr_stat.st_mtime <= _tm)
    {
        // 当前数据为新
        return 0;
    }

    return 1;
}

template <class T>
int reloader_t<T>::reload()
{
    if (_using_no != 0 && _using_no != 1) {
        return -1;
    }

    // sleep 1 秒，保证所有线程不再使用该资源
    // 外围保证
    //sleep(1);

    struct stat curr_stat;
    char fpath[sizeof(_path) + sizeof(_fname) + 1] = "";                
    snprintf(fpath, sizeof(fpath), "%s/%s.conf", _path, _fname);

    // 记录当前状态文件状态
    if ((stat(fpath, &curr_stat) != 0)
        || ((curr_stat.st_mode & S_IFREG) == 0)) {
        WARN("get file:%s status failed", fpath);
        return -1;
    }

    //T* p_t = new ( std::nothrow ) T();
    int reload_no = 1 - _using_no;
    if (_p_content[reload_no] != NULL) {
        _p_content[reload_no]->~T();
        _p_content[reload_no] = NULL;
    }
    _p_content[reload_no] = new (&_mem[reload_no * sizeof(T)]) T();

    // init
    if (_init_func != NULL) {
        int init_ret = (_p_content[reload_no]->*_init_func)();
        if (init_ret < 0) {
            FATAL("init failed.init ret:%d", init_ret);
            goto RELOAD_FAILED;
        }
    }

    {
        int load_ret = _p_content[reload_no]->load(_path, _fname);
        if (load_ret < 0) {
            FATAL("load file:%s/%s failed.load ret:%d",
                         _path, _fname, load_ret);
            goto RELOAD_FAILED;
        }
    }

    _tm = curr_stat.st_mtime;
    _last_update_tm = time(NULL);
    _using_no = reload_no;
    INFO("reload [%s] success and shift to data [%d]", fpath, _using_no);

    return 1;
RELOAD_FAILED:
    if (_p_content[reload_no] != NULL) {
        _p_content[reload_no]->~T();
        _p_content[reload_no] = NULL;
    }
    return -1;
}

template <class T>
T* reloader_t<T>::get_content()
{
    if (_tm == 0 || (_using_no != 1 && _using_no != 0)) {
        return NULL;
    }
    return _p_content[_using_no];
}

template <class T>
const T* reloader_t<T>::get_content() const
{
    return get_content();
}

#endif
