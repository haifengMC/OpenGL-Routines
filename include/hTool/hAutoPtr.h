#pragma once

namespace hTool
{
	template <typename T>
	class hAutoPtr
	{
		static std::map<T*, size_t*> pTMap;
		T* pT = NULL;
		size_t* num = NULL;//

	public:
		hAutoPtr();
		hAutoPtr(T* t);
		template< typename... Args>
		hAutoPtr(Args... args);
		hAutoPtr(const hAutoPtr& ap);
		hAutoPtr(hAutoPtr&& ap);
		~hAutoPtr();

		hAutoPtr& operator=(const hAutoPtr& ap);
		hAutoPtr& operator=(hAutoPtr&& ap);

		void bind(T* pT);
		template< typename... Args>
		void emplace(Args... args);

		operator bool() const;
		T* operator->();
		const T* operator->() const;
		T& operator*();
	private:
		void copy(const hAutoPtr& ap);
		void move(hAutoPtr&& ap);
		void destory();
	};
}