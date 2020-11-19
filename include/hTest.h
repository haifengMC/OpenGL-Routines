#pragma once
#include "hSingleton.h"

#define TEST_INIT()\
	struct testBase \
	{\
		virtual void test_title() = 0;\
		virtual void test_func() = 0;\
	};\
	class testNode : public Singleton<testNode>\
	{\
	public:\
		std::vector<testBase*> tests;\
		void addTest(testBase* pTest) { tests.push_back(pTest); }\
		void run()\
		{\
			for (testBase* pTest : tests)\
			{\
				pTest->test_title();\
				auto start = std::chrono::high_resolution_clock::now();\
				pTest->test_func();\
				auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();\
				std::cout << "<-- " << elapsed << " usec -->" << std::endl; \
			}\
		}\
	};\
	void testFunc() { testNode::getMe().run(); }

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