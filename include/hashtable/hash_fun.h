#ifndef __HASH_FUN_H
#define __HASH_FUN_H 

#include <cstddef>
#include <string>

namespace lib
{
  using std::size_t;

  template <class _Key> struct xhash { 
	  size_t operator () (const _Key & _k) const {
		  return (size_t)_k;
	  }
  };

  inline size_t
  __bsl_hash_string(const char* __s)
  {
	  if (__s == 0) return 0;
    unsigned long __h = 0;
    for ( ; *__s; ++__s)
      __h = 5*__h + *__s;
    return size_t(__h);
  }

  template <> struct xhash<std::string>  {
	  size_t operator () (const std::string & _k) const {
		  return __bsl_hash_string(_k.c_str());
	  }
  };

  template <> struct xhash<const std::string>  {
	  size_t operator () (const std::string & _k) const {
		  return __bsl_hash_string(_k.c_str());
	  }
  };

  template<> struct xhash<char*>
  {
    size_t operator()(const char* __s) const
    { return __bsl_hash_string(__s); }
  };

  template<> struct xhash<const char*>
  {
    size_t operator()(const char* __s) const
    { return __bsl_hash_string(__s); }
  };

  template<> struct xhash<char>
  { size_t operator()(char __x) const { return __x; } };

  template<> struct xhash<unsigned char>
  { size_t operator()(unsigned char __x) const { return __x; } };

  template<> struct xhash<signed char>
  { size_t operator()(unsigned char __x) const { return __x; } };

  template<> struct xhash<short>
  { size_t operator()(short __x) const { return __x; } };

  template<> struct xhash<unsigned short>
  { size_t operator()(unsigned short __x) const { return __x; } };

  template<> struct xhash<int>
  { size_t operator()(int __x) const { return __x; } };

  template<> struct xhash<unsigned int>
  { size_t operator()(unsigned int __x) const { return __x; } };

  template<> struct xhash<long>
  { size_t operator()(long __x) const { return __x; } };

  template<> struct xhash<unsigned long>
  { size_t operator()(unsigned long __x) const { return __x; } };
} // namespace __gnu_cxx

#endif
