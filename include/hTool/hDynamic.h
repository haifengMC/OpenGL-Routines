#pragma once

namespace hTool
{
	namespace hDynamicDetail
	{
		template <typename T, typename U>
		T _hDynamicCast(decltype(dynamic_cast<U>(
			static_cast<T>(NULL))) u)
		{
			return dynamic_cast<T>(u);
		}

		template <typename T, typename U>
		T _hDynamicCast(...)
		{
			static_assert((std::is_pointer<T>::value && std::is_pointer<U>::value) ||
				(std::is_reference<T>::value && std::is_reference<U>::value),
				"Be not pointer or reference");

			return NULL;
		}
	}

	template <typename T, typename U>
	T hDynamicCast(U u) { return hDynamicDetail::_hDynamicCast<T, U>(u); }
}