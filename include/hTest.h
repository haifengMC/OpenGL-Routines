#pragma once

#define TEST_INIT(testNode, testFunc)\
	class testNode : public Singleton<testNode>\
	{\
	public:\
		std::vector<void(*)()> tests;\
		void run()\
		{\
			for (void(*t)() : tests)\
				t();\
		}\
	};\
	void testFunc() { testNode::getMe().run(); }

#define TEST(testNode, testName)\
	class testName : public SingletonH<testName>\
	{\
	public:\
		testName()\
		{\
			testNode::getMe().tests.push_back(&test_title);\
			testNode::getMe().tests.push_back(&test_func);\
		}\
		static void test_title();\
		static void test_func();\
	};\
	void testName::test_title()\
	{\
		std::string s(10, '-');\
		cout << s << endl;\
		cout << TO_STRING(testName) << endl;\
		cout << s << endl;\
	}\
	void testName::test_func()