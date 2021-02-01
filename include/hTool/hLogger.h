#pragma once

namespace hTool
{
	template <typename T>
	struct Logger
	{
		static std::ostream& debug(std::ostream& os, const T& t,
			const char* tName, uint8_t n = 0, char c = '\t');
	};

	template <typename T1, typename T2>
	struct Logger<std::map<T1, T2>>
	{
		static std::ostream& debug(std::ostream& os, const std::map<T1, T2>& t,
			const char* tName, uint8_t n = 0, char c = '\t');
	};

	template <typename T>
	struct Logger<std::vector<T>>
	{
		static std::ostream& debug(std::ostream& os, const std::vector<T>& t,
			const char* tName, uint8_t n = 0, char c = '\t');
	};

	template <typename T>
	struct Logger<std::list<T>>
	{
		static std::ostream& debug(std::ostream& os, const std::list<T>& t,
			const char* tName, uint8_t n = 0, char c = '\t');
	};

	template <typename T>
	struct Logger<std::_List_iterator<std::_List_val<std::_List_simple_types<T>>>>
	{
		static std::ostream& debug(std::ostream& os, 
			const std::_List_iterator<std::_List_val<std::_List_simple_types<T>>>& t,
			const char* tName, uint8_t n = 0, char c = '\t');
	};

	template <size_t N>
	struct Logger<std::bitset<N>>
	{
		static std::ostream& debug(std::ostream& os, const std::bitset<N>& t,
			const char* tName, uint8_t n = 0, char c = '\t');
	};

#define Debug(os, va) \
	hTool::Logger<std::remove_cv<std::remove_reference<decltype(va)>::type>::type>::debug(os, va, TO_STRING(va))

#define DefLog_Init() \
		template <typename T>\
		friend struct hTool::Logger

#define DefLog_F(l, va) os<<" ";hTool::Logger<std::remove_cv<std::remove_reference<decltype(rhs.va)>::type>::type>::debug(os,rhs.va,TO_STRING(va),n,c)
#define DefLog_Cfg_F(l, va) os<<" ";hTool::Logger<std::remove_cv<std::remove_reference<decltype(rhs.data.va)>::type>::type>::debug(os,rhs.data.va,TO_STRING(va),n,c)

#define DefLog(className, ...)\
	template <> _DefLog(DefLog_F,className,##__VA_ARGS__)
#define DefLog_Template(className, ...) _DefLog(DefLog_F,className,##__VA_ARGS__)
#define DefLog_Cfg(className, ...) \
	template <> _DefLog(DefLog_Cfg_F,className,##__VA_ARGS__)
#define _DefLog(f,className, ...) \
	struct hTool::Logger<className>\
	{\
		static std::ostream& debug(\
			std::ostream& os, const className& rhs, const char* tName, \
			uint8_t n = 0, char c = '\t')\
		{\
			if (n) os << std::endl; \
				os << std::string(n++, c) << "[" << tName << "]"; \
				REPEAT_SEP(f, SEM_M, ##__VA_ARGS__); \
			return os;\
		}\
	}


}