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
    Time now;
    time_t time_of_day;
    tm time_tm;
    timer_sh.start();
    time_tm = now.get_tm();
    print_case_message("get day %lu",now.get_day());
    print_case_message("get month %lu",now.get_month());
    print_case_message("get weekday %lu",now.get_weekday());
    print_case_message("get year %lu",now.get_year());
    print_case_message("get yearday %lu",now.get_yearday());
    print_case_message("get montname %s",now.get_month_name());
    print_case_message("get time of day %lu",now.get_time_of_day());
    time_tm = now.get_tm();
    print_case_message("get second %lu",time_tm.tm_sec);
    print_case_message("get minute %lu",time_tm.tm_min);
    print_case_message("get hour %lu",time_tm.tm_hour);
    timer_sh.wait_sec(1);
    time_tm = now.get_tm();
    print_case_message("get second %lu",time_tm.tm_sec);
    print_case_message("get minute %lu",time_tm.tm_min);
    print_case_message("get hour %lu",time_tm.tm_hour);

    time_of_day = now.get_time_of_day();
    print_case_message("time of day %lu",time_of_day);
    return result;
}

bool TimeTest::execute_class_stress_case(){
    bool result = true;

    return result;
}
