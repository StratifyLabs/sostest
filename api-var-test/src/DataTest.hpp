#ifndef DATATEST_HPP
#define DATATEST_HPP

#include <sapi/var.hpp>
#include <sapi/test.hpp>

class DataTest : public Test {
public:
	DataTest();


private:

	bool execute_class_api_case();
	bool execute_class_performance_case();
	bool execute_class_stress_case();

	//API testing methods
	bool execute_fill();
	bool execute_alloc();
	bool execute_recursive(Data data);
	static int recursive_number;
};

#endif // DATATEST_HPP
