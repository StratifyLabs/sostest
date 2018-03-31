#ifndef STRINGTEST_HPP
#define STRINGTEST_HPP

#include <sapi/test.hpp>

class StringTest : public Test {
public:
    StringTest();


private:
    //cases
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();
};

#endif // STRINGTEST_HPP
