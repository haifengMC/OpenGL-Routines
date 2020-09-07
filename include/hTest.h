#pragma once
#include "hSingleton.h"

#define TEST_INIT(testNode, testFunc)\
	class testNode : public Singleton<testNode>\
	{\
	public:\
		std::vector<std::pair<void(*)(), void(*)()>> tests;\
		void run()\
		{\
			for (std::pair<void(*)(), void(*)()>& t : tests)\
			{\
				t.first();\
				auto start = std::chrono::high_resolution_clock::now();\
				t.second();\
				auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start).count();\
				std::cout << "<-- " << elapsed << " usec -->" << std::endl; \
			}\
		}\
	};\
	void testFunc() { testNode::getMe().run(); }

#define TEST(testNode, testName)\
	class testName : public SingletonH<testName>\
	{\
	public:\
		testName()\
		{\
			testNode::getMe().tests.push_back(std::make_pair(test_title, test_func));\
		}\
		static void test_title();\
		static void test_func();\
	};\
	void testName::test_title()\
	{\
		std::string s(20, '-');\
		std::cout << s << std::endl;\
		std::cout << TO_STRING(testName) << std::endl;\
		std::cout << s << std::endl;\
	}\
	void testName::test_func()