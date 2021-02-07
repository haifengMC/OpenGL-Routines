#pragma once

namespace hTool
{
	namespace hDynamicDetail
	{
		template <typename T, bool TBool, typename U, bool UBool>
		struct _hDynamicCast
		{
			static T dynamic(U u)
			{
				static_assert((std::is_pointer<T>::value && std::is_pointer<U>::value),
					"Be not pointer");

				return NULL;
			}
		};

		template <typename T, typename U>
		struct _hDynamicCast<T, true, U, true>
		{
			static T dynamic(U u) { return dynamic_cast<T>((U)u); }
		};
	}

	template <typename T, typename U>
	T hDynamicCast(U u)
	{
		return hDynamicDetail::_hDynamicCast<
			T, std::is_polymorphic<std::remove_pointer<std::remove_reference<T>::type>::type>::value,
			U, std::is_polymorphic<std::remove_pointer<std::remove_reference<U>::type>::type>::value>::
			dynamic(u);
	}
}