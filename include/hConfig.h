#pragma once
#include "vmacro.h"
#include "hYaml/hYamlEx.h"

#define CFGSTRUCT_F(n, va) node[TO_STRING(va)] & rhs.va
#define CFGDATA_F(n, va) node[TO_STRING(va)] & va
#define BEG_CFGSTRUCT(className) \
	struct className
#define END_CFGSTRUCT(className, ...) ;\
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

#define DECL_CFGDATA(className) \
	className(const std::string & fileName);\
	bool loadCfg();\
	bool saveCfg();

#define BEG_CFGDATA(className) \
	struct className : public CfgData 
#define END_CFGDATA(className, ...) ;\
	className::className(const std::string& fileName) :CfgData(fileName) {}\
	bool className::loadCfg()\
	{\
		YAML::NodeEx node;\
		if (!(fileName >> node))\
			return false;\
		node(YAML::IOType::PutIn);\
		checkMarks(node);\
		REPEAT_SEP(CFGDATA_F, SEM_M, ##__VA_ARGS__); \
		return true;\
	}\
	bool className::saveCfg()\
	{\
		YAML::NodeEx node;\
		node(YAML::IOType::PutOut);\
		REPEAT_SEP(CFGDATA_F, SEM_M, ##__VA_ARGS__); \
		setMarks(node);\
		return fileName << node;\
	}

struct CfgData
{
	struct Mark
	{
		std::list<std::string> title;
		YAML::EmitterStyle::value style;
		std::string tag;
		Mark(const std::list<std::string>& title,
			const YAML::EmitterStyle::value& style,
			const std::string& tag) : title(title), style(style), tag(tag) {}
	};

	virtual bool loadCfg() = 0;
	virtual bool saveCfg() = 0;

	CfgData(const std::string& fileName) : fileName(fileName) {}

protected:
	std::string fileName;
	std::list<Mark> marks;

	bool checkMarks(const YAML::NodeEx& node, std::list<std::string> title = std::list<std::string>());
	bool setMarks(YAML::NodeEx& node);
private:
	bool addMark(const YAML::NodeEx& node, const std::list<std::string>& title);
};

bool CfgData::checkMarks(const YAML::NodeEx& node, std::list<std::string> title)
{
	addMark(node, title);
	switch (node.Type())
	{
	case YAML::NodeType::Undefined:
	case YAML::NodeType::Null:
	case YAML::NodeType::Scalar:
		break;
	case YAML::NodeType::Sequence:
	{
		title.push_back("");
		size_t count = 0;
		for (YAML::const_iterator it = node.begin(); it != node.end(); ++it, ++count)
		{
			std::string buf = std::to_string(count);
			title.back().swap(buf);
			checkMarks(YAML::Node(*it), title);
		}
	}
	break;
	case YAML::NodeType::Map:
		title.push_back("");
		for (YAML::const_iterator it = node.begin(); it != node.end(); ++it)
		{
			std::string buf = it->first.Scalar();
			title.back().swap(buf);
			checkMarks(it->second, title);
		}
		break;
	default:
		break;
	}

	return true;
}

bool CfgData::setMarks(YAML::NodeEx& node)
{
	for (const Mark& mark : marks)
	{
		YAML::NodeEx n = node;
		for (const std::string& tt : mark.title)
			n.reset(n[tt]);

		n.SetStyle(mark.style);
		n.SetTag(mark.tag);
	}

	return true;
}

bool CfgData::addMark(const YAML::NodeEx& node, const std::list<std::string>& title)
{
	if (YAML::EmitterStyle::Default == node.Style() && node.Tag() == "?")
		return false;

	Mark m(title, node.Style(), node.Tag());
	marks.push_back(m);

	return true;
}

struct MapCfgData
{
	virtual const size_t& index() const = 0;
};