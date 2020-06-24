#ifndef SCHEDTEST_HPP
#define SCHEDTEST_HPP

#include <sapi/test.hpp>

class SchedTest : public Test {
public:
	SchedTest();
private:

	//api
	bool execute_class_api_case();
	//stress
	bool execute_class_stress_case();
	//performance
	bool execute_class_performance_case();

};

#define SET_SCHEDULER(a,b,c) TEST_THIS_EXPECT(int, \
	Sched::set_scheduler( \
	Sched::ProcessId(a), \
	b, \
	Sched::Priority(c) \
	), \
	0)

#define SET_SCHEDULER_ERROR(a,b,c,d) TEST_THIS_EXPECT_ERROR(\
	Sched::set_scheduler( \
	Sched::ProcessId(b), \
	c, \
	Sched::Priority(d) \
	), \
	a)

#endif // SCHEDTEST_HPP
