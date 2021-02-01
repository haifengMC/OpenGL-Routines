#pragma once

namespace hTool
{
	template <typename T>
	std::ostream& Logger<T>::debug(
		std::ostream& os, const T& t,
		const char* tName, uint8_t n, char c)
	{
		return os << tName << ":" << t;
	}

	template <typename T, size_t N>
	std::ostream& Logger<T[N]>::debug(
		std::ostream& os, const T(&t)[N],
		const char* tName, uint8_t n, char c)
	{
		size_t num = 0;
		os << tName << ":{";
		for (auto& i : t)
		{
			if (num) 
				os << ",";
			Logger<T>::debug(os, i, std::to_string(num++).c_str(), n, c);
		}
		
		return os << "}";
	}

	template <typename T>
	std::ostream& Logger<std::set<T>>::debug(
		std::ostream& os, const std::set<T>& t, 
		const char* tName, uint8_t n, char c)
	{
		if (t.empty())
			return os << "[" << tName << "] size:" << t.size() << " NULL";

		if (n)
			os << std::endl;

		os << std::string(n++, c) << "[" << tName << "]" <<
			" size:" << t.size() << " ";

		size_t num = 0;
		for (auto& item : t)
			Logger<T>::debug(os, item, std::to_string(num++).c_str(), n, c) << " ";

		return os;
	}

	template <typename T1, typename T2>
	std::ostream& Logger<std::map<T1, T2>>::debug(
		std::ostream& os, const std::map<T1, T2>& t, 
		const char* tName, uint8_t n, char c)
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
			Logger<T2>::debug(os, item.second, osKey.str().c_str(), n, c) << " ";
		}

		return os;
	}

	template <typename T1, typename T2>
	std::ostream& Logger<std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<T1, T2>>>>>::debug(
		std::ostream& os, const std::_Tree_iterator<std::_Tree_val<std::_Tree_simple_types<std::pair<T1, T2>>>>& t, 
		const char* tName, uint8_t n, char c)
	{
		const auto _Mycont = static_cast<const std::_Tree_val<std::_Tree_simple_types<std::pair<T1, T2>>>*>(t._Getcont());
		if (!t._Ptr || !_Mycont)
			return os << tName << ":NULL";

		if (t._Ptr == _Mycont->_Myhead)
			return os << tName << ":END";

		std::ostringstream osKey;
		Logger<T1>::debug(osKey, t->first, tName);
		return Logger<T2>::debug(os, t->second, osKey.str().c_str(), n, c);
	}

	template <typename T>
	std::ostream& Logger<std::vector<T>>::debug(
		std::ostream& os, const std::vector<T>& t, 
		const char* tName, uint8_t n, char c)
	{
		if (t.empty())
			return os << "[" << tName << "] size:" << t.size() << " NULL";

		if (n)
			os << std::endl;

		os << std::string(n++, c) << "[" << tName << "]" <<
			" size:" << t.size() << " ";

		size_t num = 0;
		for (auto& item : t)
			Logger<T>::debug(os, item, std::to_string(num++).c_str(), n, c) << " ";

		return os;
	}

	template <typename T>
	std::ostream& Logger<std::list<T>>::debug(
		std::ostream& os, const std::list<T>& t, 
		const char* tName, uint8_t n, char c)
	{
		if (t.empty())
			return os << "[" << tName << "] size:" << t.size() << " NULL";
		
		if (n)
			os << std::endl;

		os << std::string(n++, c) << "[" << tName << "]" <<
			" size:" << t.size() << " ";

		size_t num = 0;
		for (auto& item : t)
			Logger<T>::debug(os, item, std::to_string(num++).c_str(), n, c) << " ";

		return os;
	}

	template <typename T>
	std::ostream& Logger<std::_List_iterator<std::_List_val<std::_List_simple_types<T>>>>::debug(
		std::ostream& os, const std::_List_iterator<std::_List_val<std::_List_simple_types<T>>>& t, 
		 const char* tName, uint8_t n, char c)
	{
		const auto _Mycont = static_cast<const std::_List_val<std::_List_simple_types<T>>*>(t._Getcont());
		if (!t._Ptr || !_Mycont)
			return os << tName << ":NULL";

		if (t._Ptr == _Mycont->_Myhead)
			return os << tName << ":END";

		return Logger<T>::debug(os, *t, tName, n, c);
	}

	template <size_t N>
	std::ostream& Logger<std::bitset<N>>::debug(
		std::ostream& os, const std::bitset<N>& t, 
		const char* tName, uint8_t n, char c)
	{
		return os << tName << ":" << t.to_string();
	}
}
