#pragma once

namespace hTool
{
	template<typename Fst, typename Snd>
	std::ostream& operator<<(std::ostream& os, const std::pair<Fst, Snd>& p);

	class Container
	{
		std::ostream& os;
	public:
		Container(std::ostream& os);
		template<typename T>
		std::ostream& operator() (const T&);
	};
}