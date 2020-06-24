#ifndef JSONTEST_HPP
#define JSONTEST_HPP


#include <sapi/test.hpp>

class JsonTest : public Test {
public:
	JsonTest();

private:
	bool execute_class_api_case();
	bool execute_class_performance_case();
	bool execute_class_stress_case();
};

#endif // JSONTEST_HPP
