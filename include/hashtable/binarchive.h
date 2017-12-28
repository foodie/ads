#ifndef  __BINARCHIVE_H_
#define  __BINARCHIVE_H_

#include <stdlib.h>
#include <string.h>
#include "debug.h"
#include "stream.h"

namespace lib
{
class binarchive
{
protected:
	lib::stream &_fd;
public:
	binarchive(lib::stream & fd) : _fd(fd) {}
	~binarchive() {}
		
	/**
	 * @brief 从stream 流中读取一个实体数据
	 *
	 * @param [out] tp   : _Tp* 非NULL指针
	 * @return  int 成功返回0,其他失败
	 * @retval  tp传NULL 会core
	 * @see 
	 * @note 
	 * @author xiaowei
	 * @date 2008/11/04 10:17:39
	**/
	template <typename _Tp>
	int read(_Tp *tp) {
		_fd.start_trans();
		int ret = lib::deserialization(*this, *tp);
		if (ret == 0) {
			_fd.comit_trans();
		} else {
			_fd.drop_trans(false);
		}
		return ret;
	}

	/**
	 * @brief 往stream里面塞数据
	 *
	 * @param [in] tp   : const _Tp& 需要写入的实体数据
	 * @return int  成功返回0,其他失败
	 * @retval   
	 * @see 
	 * @note 
	 * @author xiaowei
	 * @date 2008/11/04 10:19:50
	**/
	template <typename _Tp>
	int write(const _Tp & tp) {
		_fd.start_trans();
		int ret = lib::serialization(*this, tp);
		if (ret == 0) {
			ret = _fd.comit_trans();
			if (ret != 0) {
				_fd.drop_trans(true);
			}
		} else {
			_fd.drop_trans(true);
		}
		return ret;
	}

public:
	/**
	 * @brief 往stream里面写入size大的数据
	 *
	 * @param [in/out] dat   : const void*
	 * @param [in/out] size   : size_t
	 * @return 0 表示成功
	 * @retval   
	 * @see 
	 * @note 
	 * @author xiaowei
	 * @date 2008/11/04 10:21:07
	**/
	int push(const void *dat, size_t size) {
		//参数检查让底层去做
		return _fd.write(dat, size);
	}

	/**
	 * @brief 往stream里面读数据
	 *
	 * @param [in/out] dat   : void*
	 * @param [in/out] size   : size_t
	 * @return  int 0表示成功,其他失败
	 * @retval   
	 * @see 
	 * @note 
	 * @author xiaowei
	 * @date 2008/11/04 10:23:02
	**/
	int pop(void *dat, size_t size) {
		return _fd.read(dat, size);
	}
};

}
#endif  //__BSL_BINARCHIVE_H_

/* vim: set ts=4 sw=4 sts=4 tw=100 */
