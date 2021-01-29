#pragma once
#include "hConfig.h"
#include "hSingleton.h"

BEG_ENUM(TestAttrType)
{
	Reverse,	//反序
	Tail,		//尾部开始
	Max
}
END_ENUM(TestAttrType, Max, Reverse)

BEG_ENUM(TestAttrTypeBit)
{
	Reverse =	0x00000001,	//反序
	Tail =		0x00000002,	//尾部开始
	Max
}
END_ENUM(TestAttrTypeBit, Max, Reverse)

#define TEST_INIT()\
	struct testBase \
	{\
		virtual void test_title() = 0;\
		virtual void test_func() = 0;\
	};\
	class testNode : public Singleton<testNode>\
	{\
		DefLog_Init();\
		size_t _showNum = 0;\
		std::bitset<TestAttrType::Max> _attr; \
	public:\
		std::vector<testBase*> tests; \
		void setAttr(const std::bitset<TestAttrType::Max>& attr) { _attr = attr; }\
		void addAttr(TestAttrType id, ...)\
		{\
			va_list pArgs = NULL; \
			va_start(pArgs, id); \
			switch (id)\
			{\
				case TestAttrType::Tail:\
					_showNum = va_arg(pArgs, size_t); \
					_attr[TestAttrType::Tail] = true;\
					break;\
				default:\
					break;\
			}\
			va_end(pArgs); \
		}\
		void addTest(testBase* pTest) { tests.push_back(pTest); }\
		void run()\
		{\
			initTest();\
			size_t totalNum = tests.size();\
			size_t begNum = \
				!_showNum || _showNum >= totalNum ? \
				0 : totalNum - _showNum;\
			for (size_t i = begNum; i < totalNum; ++i)\
			{\
				testBase* pTest = tests[i];\
				pTest->test_title(); \
				auto start = std::chrono::high_resolution_clock::now(); \
				pTest->test_func(); \
				auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count(); \
				std::cout << "<-- " << elapsed << " usec -->" << std::endl; \
			}\
		}\
	private:\
		void initTest();\
	};\
	DefLog(testNode, _showNum, _attr);\
	void testFunc() { testNode::getMe().run(); }\
	void testNode::initTest()

#define TEST(testName)\
	struct testName: public Singleton<testName>, public testBase\
	{\
		void test_title();\
		void test_func();\
	};\
	struct Reg##testName { Reg##testName(){ testNode::getMe().addTest(testName::instance()); } } reg##testName;\
	void testName::test_title()\
	{\
		std::string s(20, '-');\
		std::cout << s << std::endl;\
		std::cout << TO_STRING(testName) << std::endl;\
		std::cout << s << std::endl;\
	}\
	void testName::test_func()

#define TEST_MAIN()\
	int main()\
	{\
		testFunc();\
		return 0;\
	}