#ifndef SYSTEST_HPP
#define SYSTEST_HPP

#include <sapi/test.hpp>

class SysTest : public Test {
public:
	SysTest();
private:
	//api
	bool execute_class_api_case();
	//stress
	bool execute_class_stress_case();
	//performance
	bool execute_class_performance_case();

};

#endif // SYSTEST_HPP
