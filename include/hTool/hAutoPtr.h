#pragma once

namespace hTool
{
	template <typename T>
	class hAutoPtr;
	template <typename T>
	class hWeakPtr
	{
		T** _pPT = NULL;
	public:
		hWeakPtr(T** pPT = NULL) { _pPT = pPT; }

		operator bool() const;
		T* operator->();
		const T* operator->() const;
		T& operator*();
		const T& operator*() const;
		bool operator==(const void* pT) const;
		bool operator!=(const void* pT) const { return !operator==(pT); }
	};

	struct hAutoPtrBase
	{
		virtual void* getRaw() = 0;
		virtual void addNum() = 0;
		virtual void destory(bool desPtr) = 0;
	};

	template <typename T>
	class hAutoPtr : public hAutoPtrBase
	{
		static std::map<T*, size_t*> _pTMap;
		T** _pPT = NULL;
		size_t* _num = NULL;//

		bool _isDestory = true;
	public:
		hAutoPtr();
		hAutoPtr(T* t);
		template <typename... Args>
		hAutoPtr(Args... args);
		hAutoPtr(const hAutoPtr& ap);
		hAutoPtr(hAutoPtr&& ap);
		~hAutoPtr();

		hAutoPtr& operator=(const hAutoPtr& ap);
		hAutoPtr& operator=(hAutoPtr&& ap);
		void* getRaw() { return _pPT && *_pPT ? *_pPT : NULL; }
		void addNum() { if (_num)++* _num; }
		void destory(bool desPtr);

		void bind(T* pT);
		template <typename... Args>
		void emplace(Args... args);
		template <typename U = T>
		hWeakPtr<U> dynamic();

		operator bool() const;
		T* operator->();
		const T* operator->() const;
		T& operator*();
		const T& operator*() const;
		bool operator==(const void* pT) const;
		bool operator!=(const void* pT) const { return !operator==(pT); }

		void debug(std::ostream& os);
		static void debugMap(std::ostream& os);
	private:
		void copy(const hAutoPtr& ap, bool cpPtr = true);
		void move(hAutoPtr&& ap);
	};

	template <typename T>
	struct Logger<hAutoPtr<T>>
	{
		static std::ostream& debug(
			std::ostream& os, const hAutoPtr<T>& p, const char* tName,
			uint8_t n = 0, char c = '\t')
		{
			if (!p)
				return os << "[" << tName << "] NULL";

			return Logger<T>::debug(os, *p, tName, n, c);
		}
	};

	struct hAutoPtrObj
	{
		virtual void fillAddList(std::list<hAutoPtrBase*>&) = 0;
		virtual void fillDecList(std::list<hAutoPtrBase*>&) = 0;
		virtual void destoryPtr(const void* pT) = 0;
	};

	struct hAutoPtrRecycle
	{

	};
}