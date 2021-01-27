#pragma once

namespace hTool
{
	template <typename T>
	std::ostream& Logger<T>::debug(
		std::ostream& os, const T& t, const char* tName, uint8_t n, char c)
	{
		return os << tName << ":" << t;
	}

	template <typename T1, typename T2>
	std::ostream& Logger<std::map<T1, T2>>::debug(
		std::ostream& os, const std::map<T1, T2>& t, const char* tName, uint8_t n, char c)
	{
		if (t.empty())
			return os << "[" << tName << "] size:" << t.size() << " NULL";

		if (n)
			os << std::endl;

		os << std::string(n++, c) << "[" << tName << "]" <<
			" size:" << t.size() << " ";

		size_t num = 0;
		std::ostringstream osKey;
		for (auto& item : t)
		{
			osKey.str("");
			Logger<T1>::debug(osKey, item.first, std::to_string(num++).c_str());
			Logger<T2>::debug(os, item.second, osKey.str().c_str(), n, c);
		}

		return os;
	}

	template <typename T>
	std::ostream& Logger<std::list<T>>::debug(
		std::ostream& os, const std::list<T>& t, const char* tName, uint8_t n, char c)
	{
		if (t.empty())
			return os << "[" << tName << "] size:" << t.size() << " NULL";
		
		if (n)
			os << std::endl;

		os << std::string(n++, c) << "[" << tName << "]" <<
			" size:" << t.size() << " ";

		size_t num = 0;
		for (auto& item : t)
			Logger<T>::debug(os, item, std::to_string(num++).c_str(), n, c);
	}

	template <typename T>
	std::ostream& Logger<std::_List_iterator<std::_List_val<std::_List_simple_types<T>>>>::debug(
		std::ostream& os, const std::_List_iterator<std::_List_val<std::_List_simple_types<T>>>& t,
		const char* tName, uint8_t n, char c)
	{
		if (!t._Ptr)
			return os << tName << ":NULL";

		return Logger<T>::debug(os, *t, tName, n, c);
	}

	template <size_t N>
	std::ostream& Logger<std::bitset<N>>::debug(
		std::ostream& os, const std::bitset<N>& t, const char* tName, uint8_t n, char c)
	{
		return os << tName << ":" << t.to_string();
	}
}
