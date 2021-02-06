#pragma once

namespace hTool
{
	namespace hDynamicDetail
	{
		template <typename T, typename U>
		void _hDynamicCast(T& t,
			decltype(dynamic_cast<const volatile void*>(static_cast<U>(NULL))) u)
		{
			t = dynamic_cast<T>((U)u);
		}

		template <typename T, typename U>
		void _hDynamicCast(...) {}
	}

	template <typename T, typename U>
	T hDynamicCast(U u)
	{
		static_assert((std::is_pointer<T>::value && std::is_pointer<U>::value),
			"Be not pointer");

		T t = NULL;
		hDynamicDetail::_hDynamicCast<T, U>(t, u);
		return t;
	}
}