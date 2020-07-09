#include "global.h"

//#include "yaml-cpp/node/node.h"
//#include "yaml-cpp/parser.h"
//#include "yaml-cpp/exceptions.h"
#include "hYaml/hYamlEx.h"

namespace YAML
{
	NodeEx::NodeEx() {}
	NodeEx::NodeEx(const NodeType::value& type) : Node(type) {}

	NodeEx::NodeEx(const std::initializer_list<NodeEx>& il, const NodeType::value& type)
	{
		if (!IsSequence() && !IsMap())
		{
			if (NodeType::Sequence != type && NodeType::Map != type)
				return;

			this->reset(Node(type));
		}

		if (IsMap())
		{
			for (const NodeEx& rhs : il)
				if (rhs.IsMap())
					for (YAML::const_iterator it = rhs.begin(); it != rhs.end(); ++it)
						this->operator[](it->first) = it->second;

			return;
		}

		for (const NodeEx& rhs : il)
		{
			if (!rhs.IsSequence())
			{
				this->push_back(rhs);
				continue;
			}

			for (YAML::const_iterator it = rhs.begin(); it != rhs.end(); ++it)
				Node::push_back(*it);
		}

	}

	NodeEx::NodeEx(const Node& rhs)
	{
		AssignNode(rhs);
	}

	NodeEx::NodeEx(const NodeEx& rhs) = default;



	NodeEx& NodeEx::operator=(const Node& rhs)
	{
		AssignNode(rhs);
		return *this;
	}

	NodeEx& NodeEx::operator()(const std::string& tag)
	{
		SetTag(tag);
		return *this;
	}

	NodeEx& NodeEx::operator()(const EmitterStyle::value& style)
	{
		if (!IsSequence() && !IsMap())
			return *this;

		for (iterator it = begin(); it != end(); ++it)
		{
			const NodeType::value& type = IsSequence() ? it->Type() : it->second.Type();
			Node node = IsSequence() ? *it : it->second;
			node.SetStyle(style);
		}
		return *this;
	}

	NodeEx& NodeEx::operator()(const IOType::value& io)
	{
		m_io = io;
		return *this;
	}

	void NodeEx::push_back(const NodeEx& rhs)
	{
		const Node& node = rhs;
		Node::push_back(node);
	}

	bool operator<<(std::string fileName, const NodeEx& node)
	{
		std::ofstream fout(fileName);
		if (!fout) return false;
		fout << node;
		fout.close();
		return true;
	}

	bool operator>>(std::string fileName, NodeEx& node)
	{
		std::ifstream fin(fileName);
		if (!fin) return false;
		try
		{
			node = Load(fin);
			fin.close();
		}
		catch (const ParserException&)
		{
			fin.close();
			return false;
		}
		return true;
	}

}