#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include <sapi/fmt.hpp>
#include "FileTest.hpp"
FileTest::FileTest() : Test("sys::File"){

}
/*@brief api test for sys/File use "api-sys-test -file -api"
 */
bool File::execute_class_api_case(){
    bool result = true;
    return result;
}

/*@brief stress test for sys/File use "api-sys-test -file -stress"
 */
bool File::execute_class_stress_case(){
    bool result = true;
    return result;
}

/*@brief performance test for sys/File use "api-sys-test -file -performance"
 */
bool File::execute_class_performance_case(){
    bool result = true;
    return result;
}

