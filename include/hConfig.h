#pragma once
#include "vmacro.h"
#include "hYaml/hYamlEx.h"

template <typename T>
struct Logger
{
	static std::ostream& debug(
		std::ostream& os, const T& t, const char* tName,
		uint8_t n = 0, char c = '\t') { return os << tName << ":" << t; }
};
#define Debug(os, va) \
	Logger<decltype(va)>::debug(os, va, TO_STRING(va))

#define CFGENUM_DECODE_F(n, em, nm) case nm::em: rhs = nm::em; break
#define CFGENUM_OS_F(n, em, nm) case nm::em: return TO_STRING(em)

//enum
#define BEG_ENUM(enumName)\
	struct enumName { enum _Value

#define END_ENUM(enumName, def, ...) value;\
	enumName(_Value v = enumName::def) { value = v; }\
	operator size_t() const { return value; }\
	bool operator<(const enumName& p) const { return value < p.value; }\
	const char* getName() const{\
		switch (value){\
			REPEAT_A_SEP(CFGENUM_OS_F, 1, SEM_M, enumName, ##__VA_ARGS__);\
			default: return TO_STRING(def);}}};\
	template <>\
	struct YAML::convert<enumName>\
	{\
		static Node encode(const enumName& rhs)\
		{\
			return Node((size_t)rhs.value);\
		}\
		static bool decode(const Node& n, enumName& rhs)\
		{\
			if (!n.IsScalar())\
				return false;\
			switch (n.as<size_t>())\
			{\
				REPEAT_A_SEP(CFGENUM_DECODE_F, 1, SEM_M, enumName, ##__VA_ARGS__);\
				default:\
					rhs = enumName::def;\
					break;\
			}\
			return true;\
		}\
	};\
	template <>\
	struct Logger<enumName>\
	{\
		static std::ostream& debug(\
			std::ostream& os, const enumName& rhs, const char* tName,\
			uint8_t n = 0, char c = '\t') { return os << tName << ":" << rhs.getName(); }\
	};

//struct
//须初始化
#define CFGSTRUCT_DEBUG_F(l, va) os<<" ";Logger<decltype(rhs.data.va)>::debug(os,rhs.data.va,TO_STRING(va),n,c)
#define CFGSTRUCT_F(n, va) node[TO_STRING(va)] & rhs.data.va
#define CFGDATA_F(n, va) node[TO_STRING(va)] & data.va
#define CFGDATA_DEBUG_F(l, va) os<<std::endl;Logger<decltype(rhs.data.va)>::debug(os,rhs.data.va,TO_STRING(va),n,c)

#define BEG_CFGSTRUCT(className) \
	struct className { struct _Data
#define END_CFGSTRUCT(className, ...) \
	END_CFGSTRUCT1()\
	END_CFGSTRUCT2(className, ##__VA_ARGS__)

#define END_CFGSTRUCT1() data;
#define END_CFGSTRUCT2(className, ...) };\
	template <>\
	struct Logger<className>\
	{\
		static std::ostream& debug(\
			std::ostream& os, const className& rhs, const char* tName,\
			uint8_t n = 0, char c = '\t')\
		{\
			os << std::string(n++, c) << "[" << tName << "]"; \
			REPEAT_SEP(CFGSTRUCT_DEBUG_F, SEM_M, ##__VA_ARGS__); \
			return os;\
		}\
	};\
	template <>\
	struct YAML::convert<className>\
	{\
		static Node encode(const className& rhs)\
		{\
			NodeEx node(NodeType::Map);\
			node(IOType::PutOut);\
			REPEAT_SEP(CFGSTRUCT_F, SEM_M, ##__VA_ARGS__); \
			return node;\
		}\
		static bool decode(const Node& n, className& rhs)\
		{\
			if (!n.IsMap())\
				return false;\
			NodeEx node = n;\
			node(IOType::PutIn); \
			REPEAT_SEP(CFGSTRUCT_F, SEM_M, ##__VA_ARGS__); \
			return true;\
		}\
	};

//map
//须初始化
#define BEG_CFGMAP(className) \
	BEG_CFGSTRUCT(COMB(className, Item))
//类名-键类型-键变量名-其他数据
#define END_CFGMAP(className, KTy, KVa, ...) \
	END_CFGSTRUCT1()\
	const KTy& index() const { return data.KVa; }\
	END_CFGSTRUCT2(COMB(className, Item), KVa, ##__VA_ARGS__)\
	typedef std::map<KTy, COMB(className, Item)> className;\
	template <>\
	struct Logger<className>\
	{\
		static std::ostream& debug(\
			std::ostream& os, const className& rhs, const char* tName,\
			uint8_t n = 0, char c = '\t')\
		{\
			os << std::string(n++, c) << "[" << tName << "]" <<\
				" size:" << rhs.size(); \
			if (!rhs.empty())\
				os << std::endl; \
			size_t num = 0; \
			for (auto& item : rhs) \
			{\
				std::ostringstream osKey;\
				Logger<KTy>::debug(\
					osKey, item.first, std::to_string(num++).c_str());\
				Logger<decltype(item.second)>::debug(\
					os, item.second, osKey.str().c_str(), n, c) << std::endl; \
			}\
			return os;\
		}\
	};\
	template <>\
	struct YAML::convert<className>\
	{\
		static Node encode(const className& rhs)\
		{\
			Node node(NodeType::Sequence);\
			for (typename className::const_iterator it = rhs.begin();\
				it != rhs.end(); ++it)\
				node.push_back(it->second);\
			return node;\
		}\
		static bool decode(const Node& node, className& rhs)\
		{\
			if (!node.IsSequence())\
				return false;\
			rhs.clear();\
			for (const_iterator it = node.begin(); it != node.end(); ++it)\
			{\
				COMB(className, Item) tmp;\
				if (convert<COMB(className, Item)>::decode(*it, tmp))\
					rhs[tmp.index()] = tmp;\
			}\
			return true;\
		}\
	};

//data
//须初始化
#define BEG_CFGDATA(className) \
	struct className : public CfgData { struct _Data
#define END_CFGDATA(className, ...) data;\
	className(const std::string& fileName) :CfgData(fileName) {}\
	bool loadCfg()\
	{\
		YAML::NodeEx node;\
		if (!(fileName >> node))\
			return false;\
		node(YAML::IOType::PutIn);\
		checkMarks(node);\
		REPEAT_SEP(CFGDATA_F, SEM_M, ##__VA_ARGS__); \
		return true;\
	}\
	bool saveCfg()\
	{\
		YAML::NodeEx node;\
		node(YAML::IOType::PutOut);\
		REPEAT_SEP(CFGDATA_F, SEM_M, ##__VA_ARGS__); \
		setMarks(node);\
		return fileName << node;\
	}};\
	template <>\
	struct Logger<className>\
	{\
		static std::ostream& debug(\
		std::ostream& os, const className& rhs, const char* tName,\
		uint8_t n = 0, char c = '\t')\
		{\
			os << std::string(n++, c) << "[" << tName << "]"; \
			REPEAT_SEP(CFGDATA_DEBUG_F, SEM_M, ##__VA_ARGS__); \
			return os;\
		}\
	};

struct CfgData
{
	struct Mark
	{
		std::list<std::string> title;
		YAML::EmitterStyle::value style;
		std::string tag;
		Mark(const std::list<std::string>& title,
			const YAML::EmitterStyle::value& style,
			const std::string& tag);
	};

	virtual bool loadCfg() = 0;
	virtual bool saveCfg() = 0;

	CfgData(const std::string& fileName);

protected:
	std::string fileName;
	std::list<Mark> marks;

	bool checkMarks(const YAML::NodeEx& node, std::list<std::string> title = std::list<std::string>());
	bool setMarks(YAML::NodeEx& node);
private:
	bool addMark(const YAML::NodeEx& node, const std::list<std::string>& title);
};