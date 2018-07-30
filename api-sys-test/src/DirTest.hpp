#ifndef DIR_TEST_HPP
#define DIR_TEST_HPP

#include <sapi/test.hpp>

class DirTest:public Test {
public:
    u32 wait_time = 4000;/*!<wait time for object thread*/
    DirTest();
private:
    //api
    bool execute_class_api_case();
    //stress
    bool execute_class_stress_case();
    //performance
    bool execute_class_performance_case();

};

#endif // DIR_TEST_HPP
