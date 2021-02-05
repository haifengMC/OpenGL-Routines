#pragma once

namespace hTool
{
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
		destory(true);
	}

	template <typename T>
	hAutoPtr<T>& hAutoPtr<T>::operator=(const hAutoPtr& ap)
	{
#if(defined _D_AUTOPTR | defined _D_AUTOPTR_DETAIL)
		std::cout << "hAutoPtr<T>::operator=(const hAutoPtr& ap)" << std::endl;
#endif
		if (_pPT == ap._pPT)
			return *this;
		destory(true);
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
			destory(true);
		move(std::move(ap));

		return *this;
	}

	template<typename T>
	void hAutoPtr<T>::destory(bool desPtr)
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::destory(bool desPtr)" << std::endl;
#endif
		if (_isDestory) return;

		if (_pPT && *_pPT && desPtr)
		{
			hAutoPtrObj* pObj = hDynamicCast<hAutoPtrObj*>(*_pPT);
			if (pObj)
				pObj->destoryPtr(*_pPT);
		}

		if (_num && *_num)
		{
			--* _num;
			return;
		}

		_isDestory = true;

		if (_pPT)
		{
			if (*_pPT)
			{
				_pTMap.erase(*_pPT);
				delete* _pPT;
				*_pPT = NULL;
			}
			delete _pPT;
			_pPT = NULL;
		}

		if (_num)
		{
			delete _num;
			_num = NULL;
		}
	}

	template <typename T>
	void hAutoPtr<T>::bind(T* pT)
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::bind(T* pT)" << std::endl;
#endif
		if (!pT)
			return;

		_isDestory = false;

		if (!_pPT)
			_pPT = new T * (NULL);
		
		if (pT == *_pPT)
			return;

		if (_pPT && *_pPT)
			destory(true);

		*_pPT = pT;

		auto it = _pTMap.find(pT);
		if (it == _pTMap.end())
		{
			_num = new size_t(0);
			_pTMap.insert(std::make_pair(pT, _num));

		}
		else
		{
			_num = it->second;
			addNum();
		}
	}

	template <typename T>
	template <typename... Args>
	void hAutoPtr<T>::emplace(Args... args)
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::emplace(Args... args)" << std::endl;
#endif
		destory(true);

		_isDestory = false;

		_num = new size_t(0);
		_pPT = new T * (NULL);
		*_pPT = new T(args...);
		_pTMap.insert(std::make_pair(*_pPT, _num));
	}

	template <typename T>
	template <typename U>
	U* hAutoPtr<T>::dynamic()
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::dynamic()" << std::endl;
#endif

		return dynamic_cast<U*>(*_pPT);
	}

	template <typename T>
	hAutoPtr<T>::operator bool() const
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::operator bool()" << std::endl;
#endif
		
		return _pPT && *_pPT;
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
		for (auto& pr : _pTMap)
			os << "[" << pr.first << "]" << *pr.second << "\n";
	}

	template<typename T>
	void hAutoPtr<T>::copy(const hAutoPtr& ap, bool cpPtr)
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::copy(const hAutoPtr& ap)" << std::endl;
#endif
		_pPT = ap._pPT;
		_num = ap._num;
		_isDestory = ap._isDestory;
		addNum();

		std::list<hAutoPtrBase*> ptrList;
		do 
		{
			if (!_pPT || !*_pPT || !cpPtr)
				break;

			hAutoPtrObj* pObj = hDynamicCast<hAutoPtrObj*>(*_pPT);
			if (!pObj)
				break;

			pObj->fillCopyList(ptrList);
		} while (0);
		if (ptrList.empty())
			return;

		for (auto ptr : ptrList)
			ptr->addNum();
	}

	template<typename T>
	void hAutoPtr<T>::move(hAutoPtr&& ap)
	{
#ifdef _D_AUTOPTR_DETAIL
		std::cout << "hAutoPtr<T>::move(hAutoPtr&& ap)" << std::endl;
#endif
		_pPT = ap._pPT;
		_num = ap._num;
		_isDestory = ap._isDestory;
		ap._pPT = NULL;
		ap._num = NULL;
		ap._isDestory = true;
	}
}
