#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include <sapi/fmt.hpp>
#include "DirTest.hpp"
DirTest::DirTest() : Test("sys::Dir"){
	Data::reclaim_heap_space();
}
/*@brief api test for sys/dir use "api-sys-test -dir -api"
 */
bool DirTest::execute_class_api_case(){
	bool result = true;
	return result;
}

/*@brief stress test for sys/dir use "api-sys-test -dir -stress"
 */
bool DirTest::execute_class_stress_case(){
	bool result = true;
	return result;
}

/*@brief performance test for sys/dir use "api-sys-test -trace -performance"
 */
bool DirTest::execute_class_performance_case(){
	bool result = true;
	return result;
}

