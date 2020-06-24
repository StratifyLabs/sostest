#ifndef CLITEST_HPP
#define CLITEST_HPP

#include <sapi/test.hpp>

class CliTest:public Test {
public:
	u32 wait_time = 4000;/*!<wait time for object thread*/
	CliTest();
private:
	//api
	bool execute_class_api_case();
	//stress
	bool execute_class_stress_case();
	//performance
	bool execute_class_performance_case();

};

#endif // CLITEST_HPP
