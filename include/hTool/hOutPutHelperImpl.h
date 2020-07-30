#pragma once

namespace hTool
{
	template<typename Fst, typename Snd>
	std::ostream& operator<<(std::ostream& os, const std::pair<Fst, Snd>& p)
	{
		return os << "{" << p.first << ", " << p.second << "}";
	}

	template<typename T>
	std::ostream& Container::operator() (const T& c)
	{
		bool first = true;
		os << "{";
		for (auto& t : c)
		{
			if (first) first = false;
			else os << ", ";

			os << t;
		}
		return os << "}";
	}
}