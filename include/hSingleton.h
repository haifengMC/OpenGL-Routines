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
	Singleton(const Singleton&) = delete;
	const Singleton& operator=(const Singleton&) = delete;
protected:
	static T* pInst;
public:
	Singleton() {}
	~Singleton() {}

	static T* instance()
	{
		if (!pInst)
			pInst = TFty::instance();

		return pInst;
	}
		
	static T& getMe() { return *instance(); }
	static void delMe() { DEL(pInst); }
};
template<typename T, typename TFty>
T* Singleton<T, TFty>::pInst =NULL;

//template<typename T>
//class SingletonHFactory
//{
//public:
//	static T* instance() {  return  new T(); }
//};
//
//template<typename T, size_t N, typename TFty = SingletonHFactory<T>>
//class SingletonH
//{
//	SingletonH(const SingletonH&) = delete;
//	const SingletonH& operator=(const SingletonH&) = delete;
//protected:
//	static T* pInst;
//public:
//	SingletonH() {}
//	~SingletonH() { delMe(); }
//
//	static T& getMe() 
//	{
//		if (!pInst)
//			pInst = TFty::instance();
//
//		return *pInst;
//	}
//	static void delMe() { DEL(pInst); }
//};
//template<typename T, typename TFty>
//T* SingletonH<T, TFty>::pInst = TFty::instance();