#pragma once

#include "vcommon.h"

template<typename T>
class SingletonFactory
{
public:
	static T* instance() { return new T(); }
};

template<typename T, typename TFty = SingletonFactory<T>>
class Singleton
{
	static T* pInst;
	Singleton(const Singleton&) = delete;
	const Singleton& operator=(const Singleton&) = delete;
public:
	Singleton() {}
	~Singleton() { delMe(); }

	static T& getMe()
	{
		if (!pInst)
			pInst = TFty::instance();

		return *pInst;
	}

	static void delMe() { DEL(pInst); }
};
template<typename T, typename TFty>
T* Singleton<T, TFty>::pInst = NULL;

template<typename T>
class SingletonHFactory
{
public:
	static T& instance() {  return  *new T(); }
};

template<typename T, typename TFty = SingletonHFactory<T>>
class SingletonH
{
	static T& inst;
	SingletonH(const SingletonH&) = delete;
	const SingletonH& operator=(const SingletonH&) = delete;
public:
	SingletonH() {}
	~SingletonH() { delMe(); }

	static T& getMe() { return inst; }
	static void delMe() { delete& inst; }
};
template<typename T, typename TFty>
T& SingletonH<T, TFty>::inst = TFty::instance();;