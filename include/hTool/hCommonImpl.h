#pragma once

namespace hTool
{
	template<typename T>
	bool isConstructible(Constructible c)
	{
		switch (c)
		{
		case Constructible::Default:
			return std::is_default_constructible<T>::value;
		case  Constructible::Copy:
			return std::is_copy_constructible<T>::value;
		case Constructible::Move:
			return std::is_move_constructible<T>::value;
		default:
			break;
		}

		return false;
	}

	template<typename T>
	bool isConstructible(const T& t, Constructible c)
	{
		return isConstructible<T>(c);
	}
}