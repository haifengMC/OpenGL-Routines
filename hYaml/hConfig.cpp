#include "hConfig.h"

CfgData::Mark::Mark(const std::list<std::string>& title,
	const YAML::EmitterStyle::value& style,
	const std::string& tag) : title(title), style(style), tag(tag) {}

CfgData::CfgData(const std::string& fileName) : fileName(fileName) {}

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
			std::string buf = "seq:";
			buf.append(std::to_string(count));
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
		{
			std::string seq = "seq:";
			if (tt.compare(0, seq.size(), seq))
				n.reset(n[tt]);
			else
			{
				size_t idx = std::stoul(tt.substr(seq.size()));
				n.reset(n[idx]);
			}
		}

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