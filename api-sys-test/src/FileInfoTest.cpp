#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include <sapi/fmt.hpp>
#include "FileInfoTest.hpp"
FileInfoTest::FileInfoTest() : Test("sys::FileInfo"){
	Data::reclaim_heap_space();
}
/*@brief api test for sys/FileInfo use "api-sys-test -file_info -api"
 */
bool FileInfoTest::execute_class_api_case(){
    bool result = true;
    return result;
}

/*@brief stress test for sys/FileInfo use "api-sys-test -file_info -stress"
 */
bool FileInfoTest::execute_class_stress_case(){
    bool result = true;
    return result;
}

/*@brief performance test for sys/FileInfo use "api-sys-test -file_info -performance"
 */
bool FileInfoTest::execute_class_performance_case(){
    bool result = true;
    return result;
}

