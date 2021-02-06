#pragma once

namespace hTool
{
	template <typename T>
	class hWeakPtr
	{
		DefLog_Init();
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

	template <typename T>
	struct Logger<hWeakPtr<T>>
	{
		static std::ostream& debug(
			std::ostream& os, const hWeakPtr<T>& p, const char* tName,
			uint8_t n = 0, char c = '\t')
		{
			if (!p)
				return os << tName << ":NULL";

			return os << tName << ":" << p._pPT;
		}
	};

	template <typename T>
	class hAutoPtr
	{
		static std::map<T*, size_t*> _pTMap;
		T** _pPT = NULL;
		size_t* _num = NULL;//
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
		void copy(const hAutoPtr& ap);
		void move(hAutoPtr&& ap);
		void destory();
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

	template <typename T>
	class hAutoPtrObj
	{
		friend class hAutoPtr<T>;
		hWeakPtr<T> thisPtr;
	protected:
		hWeakPtr<T>& getThis() { return thisPtr; };
	public:
		~hAutoPtrObj() {}
	};
}