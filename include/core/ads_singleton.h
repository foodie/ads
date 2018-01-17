#ifndef _ADS_SINGLETON_H
#define _ADS_SINGLETON_H

/**
 * 单例类
 */

template <typename T>
class AdsSingleton
{
private:
	AdsSingleton(const AdsSingleton& src) {}
	AdsSingleton &operator=(const AdsSingleton& src) {}

	/* 构造器 */
	class Creator
	{
	public:
		Creator() { AdsSingleton<T>::getInstance(); }
		inline void doNothing() const {}
	};
	static Creator creator;

protected:
	AdsSingleton(){};

public:
	static T& getInstance()
	{
		static T obj;
		creator.doNothing();
		return obj;
	}
};

template <typename T>
typename AdsSingleton<T>::Creator AdsSingleton<T>::creator;

#endif
/* vim: set ts=4 sw=4 noet: */
