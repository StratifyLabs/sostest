#include "TimeTest.hpp"
#include "sapi/chrono.hpp"
#include "ctime"
TimeTest::TimeTest() : Test("chrono::Time"){

}

/*! \details test "api" a chrono::Time
 *  constructors,
 * @return false if some test failed
 */
bool TimeTest::execute_class_api_case(){
    bool result = true;
    MicroTimer timer_sh;
    Time now(0,18,18);
    time_t time_of_day;
    timer_sh.start();
    time_of_day = now.get_time_of_day();
    timer_sh.wait_sec(1);
    time_of_day = now.get_time_of_day();
    return result;
}

bool TimeTest::execute_class_stress_case(){
    bool result = true;

    return result;
}
