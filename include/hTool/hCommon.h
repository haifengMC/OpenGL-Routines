#pragma once

namespace hTool
{
	enum class Constructible
	{
		Default,
		Copy,
		Move,
	};

	template<typename T>
	bool isConstructible(Constructible);
	template<typename T>
	bool isConstructible(const T&, Constructible);
}