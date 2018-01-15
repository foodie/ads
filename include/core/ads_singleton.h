#ifndef _ADS_SINGLETON_H
#define _ADS_SINGLETON_H

/**
 * 单例类
 */

template <typename T>
class AdsSingleton
{
protected:
	struct object_creator
	{
		object_creator() { ssingleton<T>::instance(); }
		inline void do_nothing() const { }
	};
	static object_creator create_object;
	AdsSingleton(){};
public:
	typedef T object_type;
	static object_type & instance()
	{
		static object_type obj;
		create_object.do_nothing();
		return obj;
	}
	static object_type& getInstance()
	{
		return instance();
	}
};

template <typename T>
typename ssingleton<T>::object_creator ssingleton<T>::create_object;

#endif
/* vim: set ts=4 sw=4 noet: */
