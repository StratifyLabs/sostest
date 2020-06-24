#ifndef FILE_INFO_TEST_HPP
#define FILE_INFO_TEST_HPP

#include <sapi/test.hpp>

class FileInfoTest:public Test {
public:
	u32 wait_time = 4000;/*!<wait time for object thread*/
	FileInfoTest();
private:
	//api
	bool execute_class_api_case();
	//stress
	bool execute_class_stress_case();
	//performance
	bool execute_class_performance_case();

};

#endif // FILE_INFO_TEST_HPP
