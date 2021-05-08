#pragma once

namespace hTool
{
#if 0
	template <typename T>
	template <typename U>
	hWeakPtr<T>::hWeakPtr(U* u)
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hWeakPtr<T>::hWeakPtr(U* u)" << std::endl;
#endif
		hAutoPtr<U> pU(u);
		if (!pU.getNum())
			return;

		T* pT = hDynamicCast<T*>(u);
		if (!pT)
			return;

		*this = pU.dynamic<T>();
	}
#endif

	template <typename T>
	template <typename U>
	hWeakPtr<U> hWeakPtr<T>::dynamic()
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hWeakPtr<T>::dynamic()" << std::endl;
#endif

		if (!_pPT)
			return hWeakPtr<U>();

		U* pU = hDynamicCast<U*>(*_pPT);
		if (pU)
			return hWeakPtr<U>((U**)_pPT);

		return hWeakPtr<U>();
	}

	template <typename T>
	template <typename U>
	hWeakPtr<const U> hWeakPtr<T>::dynamic() const
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hWeakPtr<const T>::dynamic() const" << std::endl;
#endif

		if (!_pPT)
			return hWeakPtr<const U>();

		const U* pU = hDynamicCast<const U*>(*_pPT);
		if (pU)
			return hWeakPtr<const U>((const U**)_pPT);

		return hWeakPtr<const U>();
	}

	template <typename T>
	hWeakPtr<T>::operator bool() const
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator bool()" << std::endl;
#endif

		return _pPT && *_pPT;
	}

	template<typename T>
	T* hWeakPtr<T>::operator->()
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator->()" << std::endl;
#endif
		if (!_pPT || !*_pPT)
			throw std::runtime_error("¿ÕÖ¸Õë");

		return *_pPT;
	}

	template<typename T>
	const T* hWeakPtr<T>::operator->() const
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator->() const" << std::endl;
#endif
		if (!_pPT || !*_pPT)
			throw std::runtime_error("¿ÕÖ¸Õë");

		return *_pPT;
	}

	template<typename T>
	T& hWeakPtr<T>::operator*()
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator*()" << std::endl;
#endif
		if (!_pPT || !*_pPT)
			throw std::runtime_error("¿ÕÖ¸Õë");

		return **_pPT;
	}

	template<typename T>
	const T& hWeakPtr<T>::operator*() const
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator*() const" << std::endl;
#endif
		if (!_pPT || !*_pPT)
			throw std::runtime_error("¿ÕÖ¸Õë");

		return **_pPT;
	}

	template<typename T>
	bool hWeakPtr<T>::operator==(const void* pT) const
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator==(const void*) const" << std::endl;
#endif
		if (!_pPT || !*_pPT)
		{
			if (pT) return false;
			else return true;
		}

		return *_pPT == pT;
	}

	template <typename T>
	std::map<T*, size_t*> hAutoPtr<T>::_pTMap;

	template <typename T>
	hAutoPtr<T>::hAutoPtr() 
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::hAutoPtr()" << std::endl;
#endif
	}

	template <typename T>
	hAutoPtr<T>::hAutoPtr(T* t) 
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::hAutoPtr(T* t)" << std::endl;
#endif
		bind(t); 
	}

	template <typename T>
	template < typename... Args>
	hAutoPtr<T>::hAutoPtr(Args... args)
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::~hAutoPtr()" << std::endl;
#endif
		emplace(args ...);
	}

	template <typename T>
	hAutoPtr<T>::hAutoPtr(const hAutoPtr& ap) 
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::hAutoPtr(const hAutoPtr& ap)" << std::endl;
#endif
		copy(ap); 
	}

	template <typename T>
	hAutoPtr<T>::hAutoPtr(hAutoPtr&& ap) 
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::hAutoPtr(hAutoPtr&& ap)" << std::endl;
#endif
		move(std::move(ap)); 
	}

	template <typename T>
	hAutoPtr<T>::~hAutoPtr()
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::~hAutoPtr()" << std::endl;
#endif
		destory();
	}

	template<typename T>
	void hAutoPtr<T>::destory()
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::destory()" << std::endl;
#endif
		if (_num && *_num)
		{
			--* _num;
			return;
		}

		do
		{
			if (!_pPT)
				break;

			T** pPT = NULL;
			std::swap(_pPT, pPT);

			if (!*pPT)
				break;

			T* pT = NULL;
			std::swap(pT, *pPT);

			_pTMap.erase(pT);
			delete pT;

		} while (0);

		if (_num)
		{
			delete _num;
			_num = NULL;
		}
	}

	template <typename T>
	hAutoPtr<T>& hAutoPtr<T>::operator=(const hAutoPtr& ap)
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::operator=(const hAutoPtr& ap)" << std::endl;
#endif
		if (_pPT == ap._pPT)
			return *this;
		destory();
		copy(ap);

		return *this;
	}

	template <typename T>
	hAutoPtr<T>& hAutoPtr<T>::operator=(hAutoPtr&& ap)
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::operator=(hAutoPtr&& ap)" << std::endl;
#endif
		if (_pPT && *_pPT)
			destory();
		move(std::move(ap));

		return *this;
	}

	template <typename T>
	void hAutoPtr<T>::bind(T* pT)
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::bind(T* pT)" << std::endl;
#endif
		if (!pT)
			return;

		if (_pPT && *_pPT)
			destory();

		if (!_pPT)
			_pPT = new T * (NULL);
		
		if (pT == *_pPT)
			return;

		*_pPT = pT;
		doPtrObj();

		auto it = _pTMap.find(pT);
		if (it == _pTMap.end())
		{
			_num = new size_t(0);
			_pTMap.insert(std::make_pair(pT, _num));

		}
		else
		{
			_num = it->second;
			if (_num)++* _num;
		}
	}

	template <typename T>
	template <typename... Args>
	void hAutoPtr<T>::emplace(Args... args)
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::emplace(Args... args)" << std::endl;
#endif
		destory();

		_num = new size_t(0);
		_pPT = new T * (NULL);
		*_pPT = new T(args...);
		doPtrObj();

		_pTMap.insert(std::make_pair(*_pPT, _num));
	}

	template <typename T>
	template <typename U>
	hWeakPtr<U> hAutoPtr<T>::dynamic()
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::dynamic()" << std::endl;
#endif
		if (!_pPT)
			return hWeakPtr<U>();

		U* pU = hDynamicCast<U*>(*_pPT);
		if (pU)
			return hWeakPtr<U>((U**)_pPT);

		return hWeakPtr<U>();
	}

	template <typename T>
	template <typename U>
	hWeakPtr<const U> hAutoPtr<T>::dynamic() const
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<const T>::dynamic() const" << std::endl;
#endif
		if (!_pPT)
			return hWeakPtr<const U>();

		const U* pU = hDynamicCast<const U*>(*_pPT);
		if (pU)
			return hWeakPtr<const U>((const U**)_pPT);

		return hWeakPtr<const U>();
	}

	template <typename T>
	size_t hAutoPtr<T>::getNum() const
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<const T>::getNum() const" << std::endl;
#endif

		if (!_num)
			return 0;

		return *_num;
	}


	template <typename T>
	hAutoPtr<T>::operator bool() const
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator bool()" << std::endl;
#endif

		return _pPT && *_pPT;
	}

	template <typename T>
	template <typename U>
	hAutoPtr<T>::operator hWeakPtr<U>()
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator hWeakPtr<U>()" << std::endl;
#endif

		return dynamic<U>();
	}

	template<typename T>
	T* hAutoPtr<T>::operator->()
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator->()" << std::endl;
#endif
		if (!_pPT || !*_pPT)
			throw std::runtime_error("¿ÕÖ¸Õë");

		return *_pPT;
	}

	template<typename T>
	const T* hAutoPtr<T>::operator->() const
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator->() const" << std::endl;
#endif
		if (!_pPT || !*_pPT)
			throw std::runtime_error("¿ÕÖ¸Õë");

		return *_pPT;
	}

	template<typename T>
	T& hAutoPtr<T>::operator*()
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator*()" << std::endl;
#endif
		if (!_pPT || !*_pPT)
			throw std::runtime_error("¿ÕÖ¸Õë");

		return **_pPT;
	}

	template<typename T>
	const T& hAutoPtr<T>::operator*() const
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator*() const" << std::endl;
#endif
		if (!_pPT || !*_pPT)
			throw std::runtime_error("¿ÕÖ¸Õë");

		return **_pPT;
	}

	template<typename T>
	bool hAutoPtr<T>::operator==(const void* pT) const
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator==(const void*) const" << std::endl;
#endif
		if (!_pPT || !*_pPT)
		{
			if (pT) return false;
			else return true;
		}

		return *_pPT == pT;
	}

	template<typename T>
	void hAutoPtr<T>::debug(std::ostream& os)
	{
		os << typeid(T).name() << "\n";
		os << "pT:" << (_pPT && *_pPT ? *_pPT : "NULL") << ",num:" << (_num ? *_num : 0) << "\n";
		hAutoPtr<T>::debugMap(os);
	}

	template<typename T>
	void hAutoPtr<T>::debugMap(std::ostream& os)
	{
		os << typeid(T).name() << "\n";
		for (auto& pr : _pTMap)
			os << "[" << pr.first << "]" << *pr.second << "\n";
	}

	template<typename T>
	void hAutoPtr<T>::copy(const hAutoPtr& ap)
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::copy(const hAutoPtr& ap)" << std::endl;
#endif
		_pPT = ap._pPT;
		_num = ap._num;
		if (_num)++* _num;

		if (!_pPT || !*_pPT)
			return;
		doPtrObj();
	}

	template<typename T>
	void hAutoPtr<T>::move(hAutoPtr&& ap)
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::move(hAutoPtr&& ap)" << std::endl;
#endif
		_pPT = ap._pPT;
		_num = ap._num;
		ap._pPT = NULL;
		ap._num = NULL;

		if (!_pPT || !*_pPT)
			return;

		doPtrObj();
	}

	template<typename T>
	void hAutoPtr<T>::doPtrObj()
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::doPtrObj(T* pT)" << std::endl;
#endif
		hAutoPtrObj* pObj = hDynamicCast<hAutoPtrObj*>(*_pPT);
		if (!pObj)
			return;
			
		pObj->thisPtr.bind(new hWeakPtr<T>(_pPT));
	}

	template <typename T>
	hWeakPtr<T> hAutoPtrObj::getThis()
	{
		auto ptr = thisPtr.dynamic<hWeakPtr<T>>();
		if (!ptr)
			return hWeakPtr<T>();

		return *ptr;
	}
}
