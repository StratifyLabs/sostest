#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include <sapi/fmt.hpp>
#include "AioTest.hpp"
AioTest::AioTest() : Test("sys::Aio"){
	Data::reclaim_heap_space();
}
/*@brief api test for sys/aio use "api-sys-test -aio -api"
 */
bool AioTest::execute_class_api_case(){
	bool result = true;
	return result;
}

/*@brief stress test for sys/aio use "api-sys-test -trace -stress"
 */
bool AioTest::execute_class_stress_case(){
	bool result = true;
	return result;
}

/*@brief performance test for sys/aio use "api-sys-test -trace -performance"
 */
bool AioTest::execute_class_performance_case(){
	bool result = true;
	return result;
}

