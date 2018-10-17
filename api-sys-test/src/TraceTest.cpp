#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include <sapi/fmt.hpp>
#include "TraceTest.hpp"
TraceTest::TraceTest() : Test("sys::Trace"){
	Data::reclaim_heap_space();
}
/*@brief api test for sys/trace use "api-sys-test -trace -api"
 * 	Trace& operator=(const char * a)
 *  Trace& operator=(const var::String & a)
 *	trace_message()
 *	trace_warning()
 *	trace_error()
 *	trace_critical()
 *	trace_fatal()
 */
bool TraceTest::execute_class_api_case(){
    bool result = true;
    Trace trace_test;
    String message("String");
    trace_test = message;
    trace_test.trace_message();
    trace_test.trace_warning();
    trace_test.trace_error();
    trace_test.trace_critical();
    trace_test.trace_fatal();
    trace_test = "Native";
    trace_test.trace_message();
    trace_test.trace_warning();
    trace_test.trace_error();
    trace_test.trace_critical();
    trace_test.trace_fatal();
    trace_test = "";
    trace_test.trace_message();
    trace_test.trace_warning();
    trace_test.trace_error();
    trace_test.trace_critical();
    trace_test.trace_fatal();
    return result;
}

/*@brief stress test for sys/trace use "api-sys-test -trace -stress"
 */
bool TraceTest::execute_class_stress_case(){
    bool result = true;
    return result;
}

/*@brief performance test for sys/trace use "api-sys-test -trace -performance"
 */
bool TraceTest::execute_class_performance_case(){
    bool result = true;
    return result;
}

