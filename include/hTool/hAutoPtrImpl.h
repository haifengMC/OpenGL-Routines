#pragma once

namespace hTool
{
	template <typename T>
	std::map<T*, size_t*> hAutoPtr<T>::pTMap;

	template<typename T>
	hAutoPtr<T>::hAutoPtr() 
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::hAutoPtr()" << std::endl;
#endif
	}

	template<typename T>
	hAutoPtr<T>::hAutoPtr(T* t) 
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::hAutoPtr(T* t)" << std::endl;
#endif
		bind(t); 
	}

	template<typename T>
	hAutoPtr<T>::~hAutoPtr() 
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::~hAutoPtr()" << std::endl;
#endif
		destory();
	}

	template<typename T>
	hAutoPtr<T>::hAutoPtr(const hAutoPtr& ap) 
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::hAutoPtr(const hAutoPtr& ap)" << std::endl;
#endif
		copy(ap); 
	}

	template<typename T>
	hAutoPtr<T>::hAutoPtr(hAutoPtr&& ap) 
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::hAutoPtr(hAutoPtr&& ap)" << std::endl;
#endif
		move(std::move(ap)); 
	}

	template<typename T>
	hAutoPtr<T>& hAutoPtr<T>::operator=(const hAutoPtr& ap)
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::operator=(const hAutoPtr& ap)" << std::endl;
#endif
		if (pT == ap.pT)
			return *this;
		destory();
		copy(ap);

		return *this;
	}

	template<typename T>
	hAutoPtr<T>& hAutoPtr<T>::operator=(hAutoPtr&& ap)
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::operator=(hAutoPtr&& ap)" << std::endl;
#endif
		if (!pT)
			destory();
		move(std::move(ap));

		return *this;
	}

	template<typename T>
	void hAutoPtr<T>::bind(T* pT)
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::bind(T* pT)" << std::endl;
#endif
		if (!pT)
			return;

		if (pT == this->pT)
			return;

		destory();

		this->pT = pT;

		auto it = pTMap.find(pT);
		if (it == pTMap.end())
		{
			num = new size_t(0);
			pTMap.insert(std::make_pair(pT, num));

		}
		else
		{
			num = it->second;
			++* num;
		}
	}

	template<typename T>
	template<typename... Args>
	void hAutoPtr<T>::emplace(Args... args)
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::emplace(Args... args)" << std::endl;
#endif
		destory();

		num = new size_t(0);
		pT = new T(args...);
		pTMap.insert(std::make_pair(pT, num));
	}

	template<typename T>
	hAutoPtr<T>::operator bool() const
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator bool()" << std::endl;
#endif
		return pT;
	}

	template<typename T>
	T* hAutoPtr<T>::operator->() 
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator->()" << std::endl;
#endif
		if (!pT)
			abort();

		return pT;
	}

	template<typename T>
	const T* hAutoPtr<T>::operator->() const 
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator->() const" << std::endl;
#endif
		if (!pT)
			abort();

		return pT;
	}

	template<typename T>
	T& hAutoPtr<T>::operator*() 
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator*()" << std::endl;
#endif
		if (!pT)
			abort();

		return *pT;
	}

	template<typename T>
	void hAutoPtr<T>::copy(const hAutoPtr& ap)
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::copy(const hAutoPtr& ap)" << std::endl;
#endif
		pT = ap.pT;
		num = ap.num;
		++* num;
	}

	template<typename T>
	void hAutoPtr<T>::move(hAutoPtr&& ap)
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::move(hAutoPtr&& ap)" << std::endl;
#endif
		pT = ap.pT;
		num = ap.num;
		ap.pT = NULL;
		ap.num = NULL;
	}

	template<typename T>
	void hAutoPtr<T>::destory()
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::destory()" << std::endl;
#endif
		if (num && *num)
		{
			--* num;
			return;
		}

		if (pT)
		{
			delete pT;
			pTMap.erase(pT);
			pT = NULL;
		}

		if (num)
		{
			delete num;
			num = NULL;
		}
	}
}
