#ifndef  __SERIALIZATION_H_
#define  __SERIALIZATION_H_

#include <malloc.h>
#include <string>
#include <utility>
#include "debug.h"

namespace lib
{

template <class _Archive, typename _Tp>
int serialization(_Archive & ar, const _Tp & tp)
{
	return ((_Tp &)tp).serialization(ar);
}

template <class _Archive, typename _Tp>
int deserialization(_Archive & ar, _Tp &tp)
{
	return tp.deserialization(ar);
}

#define __BSL_SERIAL_DEF(type) \
template <class _Archive> \
int serialization(_Archive & ar, type &tp) \
{ \
	return ar.push(&tp, sizeof(tp)); \
} \

#define __BSL_DESERIAL_DEF(type) \
template <class _Archive> \
int deserialization(_Archive &ar, type &tp) \
{ \
	return ar.pop(&tp, sizeof(tp)); \
}

__BSL_SERIAL_DEF(char);
__BSL_SERIAL_DEF(const char);
__BSL_SERIAL_DEF(unsigned char);
__BSL_SERIAL_DEF(const unsigned char);
__BSL_SERIAL_DEF(short);
__BSL_SERIAL_DEF(const short);
__BSL_SERIAL_DEF(unsigned short);
__BSL_SERIAL_DEF(const unsigned short);
__BSL_SERIAL_DEF(int);
__BSL_SERIAL_DEF(const int);
__BSL_SERIAL_DEF(unsigned int);
__BSL_SERIAL_DEF(const unsigned int);
__BSL_SERIAL_DEF(long);
__BSL_SERIAL_DEF(const long);
__BSL_SERIAL_DEF(unsigned long);
__BSL_SERIAL_DEF(const unsigned long);
__BSL_SERIAL_DEF(float);
__BSL_SERIAL_DEF(const float);
__BSL_SERIAL_DEF(double);
__BSL_SERIAL_DEF(const double);
__BSL_SERIAL_DEF(long long);
__BSL_SERIAL_DEF(const long long);
__BSL_SERIAL_DEF(unsigned long long);
__BSL_SERIAL_DEF(const unsigned long long);

__BSL_DESERIAL_DEF(char);
__BSL_DESERIAL_DEF(unsigned char);
__BSL_DESERIAL_DEF(short);
__BSL_DESERIAL_DEF(unsigned short);
__BSL_DESERIAL_DEF(int);
__BSL_DESERIAL_DEF(unsigned int);
__BSL_DESERIAL_DEF(long);
__BSL_DESERIAL_DEF(unsigned long);
__BSL_DESERIAL_DEF(float);
__BSL_DESERIAL_DEF(double);
__BSL_DESERIAL_DEF(long long);
__BSL_DESERIAL_DEF(unsigned long long);

//增加std:string的序列化函数
template <class _Archive>
int serialization(_Archive &ar, const std::string & str)
{
	size_t siz = str.size();
	if (ar.push(&siz, sizeof(siz)) != 0) {
		__BSL_ERROR("push siz[%ld] err", (long)siz);
		return -1;
	}
	//__BSL_DEBUG("push siz=%ld", (long)siz);
	if (ar.push(str.c_str(), str.size()) != 0) {
		__BSL_ERROR("push str[%s] err", str.c_str());
		return -1;
	}
	return 0;
}

template <class _Archive>
int deserialization(_Archive &ar, std::string & str)
{
	size_t siz = 0;
	if (ar.pop(&siz, sizeof(siz)) != 0) {
		__BSL_ERROR("pop siz err");
		return -1;
	}
	//__BSL_DEBUG("pop siz=%ld", (long)siz);
	char *ptr = (char *)malloc(siz+1);
	if (ptr == NULL) {
		return -1;
	}
	ptr[siz] = 0;
	if (ar.pop(ptr, siz) != 0) {
		__BSL_ERROR("pop str err");
		free (ptr);
		return -1;
	}
	//__BSL_DEBUG("pop str=%s", ptr);
	str = ptr;
	free (ptr);
	return 0;
}

template <class _Archive, typename _T1, typename _T2>
int serialization(_Archive &ar, const std::pair<_T1, _T2> &__p)
{
	if (lib::serialization(ar, __p.first) != 0) return -1;
	if (lib::serialization(ar, __p.second) != 0) return -1;
	return 0;
}

template <class _Archive, typename _T1, typename _T2>
int deserialization(_Archive &ar, std::pair<_T1, _T2> &__p)
{
	if (lib::deserialization(ar, __p.first) != 0) return -1;
	if (lib::deserialization(ar, __p.second) != 0) return -1;
	return 0;
}

};

#endif

/* vim: set ts=4 sw=4 sts=4 tw=100 */
