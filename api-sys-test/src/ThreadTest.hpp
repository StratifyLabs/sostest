#ifndef THREADTEST_HPP
#define THREADTEST_HPP
#include <sapi/sys.hpp>
#include <sapi/test.hpp>
#include <sapi/chrono.hpp>

class ThreadTest : public Test {
public:
	ThreadTest();

private:
	//api
	static int count_1;
	static int count_2;
	static int count_3;
	u32 wait_time = 500000;

	bool execute_class_api_case();
	//stress
	bool execute_class_stress_case();
	//performance
	bool execute_class_performance_case();
	static void * handle_thread_1(void * args){
		ThreadTest * object = (ThreadTest*)args;
		return object->thread_1(object->wait_time);
	}
	static void * handle_thread_2(void * args){
		ThreadTest * object = (ThreadTest*)args;
		return object->thread_2(object->wait_time);
	}
	static void * handle_thread_3(void * args){
		ThreadTest * object = (ThreadTest*)args;
		return object->thread_3(object->wait_time);
	}

	Timer m_thread_2_timer;
	int m_thread_2_errno;

	void * thread_1(u32);
	void * thread_2(u32);
	void * thread_3(u32);
};
enum Sched::policy test_get_policy(int value);

#define CREATE_THREAD(a,b,c,d,e) TEST_THIS_ASSERT(bool, \
	a.create( \
	Thread::Function(b), \
	Thread::FunctionArgument(c), \
	Thread::Priority(d), \
	e \
	) < 0, \
	false)

#define CREATE_THREAD_ERROR(f,a,b,c,d,e) TEST_THIS_EXPECT_ERROR(\
	a.create( \
	Thread::Function(b), \
	Thread::FunctionArgument(c), \
	Thread::Priority(d), \
	e \
	) < 0, \
	f)


#endif // THREADTEST_HPP
