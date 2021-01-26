#pragma once

namespace hTool
{
	template <typename T>
	struct Logger
	{
		static std::ostream& debug(
			std::ostream& os, const T& t, const char* tName,
			uint8_t n = 0, char c = '\t') {
			return os << tName << ":" << t;
		}
	};

	template <typename T1, typename T2>
	struct Logger<std::map<T1, T2>>
	{
		static std::ostream& debug(
			std::ostream& os, const std::map<T1, T2>& t, const char* tName,
			uint8_t n = 0, char c = '\t')
		{
			if (n)
				os << std::endl;
			os << std::string(n++, c) << "[" << tName << "]" <<
				" size:" << t.size();
			if (t.empty())
				return os << " NULL";

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
	};
#define Debug(os, va) \
	hTool::Logger<decltype(va)>::debug(os, va, TO_STRING(va))

#define DefLog_F(l, va) os<<" ";hTool::Logger<decltype(rhs.va)>::debug(os,rhs.va,TO_STRING(va),n,c)
#define DefLog_CFG_F(l, va) os<<" ";hTool::Logger<decltype(rhs.data.va)>::debug(os,rhs.data.va,TO_STRING(va),n,c)

#define DefLog(className, ...) _DefLog(DefLog_F,className,##__VA_ARGS__)
#define DefLog_CFG(className, ...) _DefLog(DefLog_CFG_F,className,##__VA_ARGS__)
#define _DefLog(f,className, ...) \
	template <>\
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