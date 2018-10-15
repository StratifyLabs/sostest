#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include <sapi/fmt.hpp>
#include "FileTest.hpp"
FileTest::FileTest() : Test("sys::File"){
	Data::reclaim_heap_space();
}
/*@brief api test for sys/File use "api-sys-test -file -api"
 */
bool FileTest::execute_class_api_case(){
    bool result = true;
    return result;
}

/*@brief stress test for sys/File use "api-sys-test -file -stress"
 */
bool FileTest::execute_class_stress_case(){
    bool result = true;
    return result;
}

/*@brief performance test for sys/File use "api-sys-test -file -performance"
 */
bool FileTest::execute_class_performance_case(){
    bool result = true;
    return result;
}

