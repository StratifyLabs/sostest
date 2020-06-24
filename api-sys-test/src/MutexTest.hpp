#ifndef MUTEXTEST_HPP
#define MUTEXTEST_HPP
#include <sapi/test.hpp>

class MutexTest:public Test{
public:
	MutexTest();
private:
	//api
	bool execute_class_api_case();
	//stress
	bool execute_class_stress_case();
	//performance
	bool execute_class_performance_case();
	//mutex attr
	bool execute_api_mutex_attr_case(MutexAttr * mutex_attr);
};

#endif // MUTEXTEST_HPP
