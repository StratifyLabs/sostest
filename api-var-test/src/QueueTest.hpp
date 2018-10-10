#ifndef QUEUETEST_HPP
#define QUEUETEST_HPP

#include <sapi/test.hpp>

class QueueTest : public Test {
public:
	QueueTest();

private:
	bool execute_class_api_case();
	bool execute_class_performance_case();
	bool execute_class_stress_case();
};

#endif // QUEUETEST_HPP
