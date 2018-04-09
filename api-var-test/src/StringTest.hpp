#ifndef STRINGTEST_HPP
#define STRINGTEST_HPP

#include <sapi/test.hpp>

class StringTest : public Test {
public:
    StringTest();


private:
    //cases
    bool execute_class_api_case();
    bool api_case_assign();
    bool api_case_compare();
    bool api_case_find();
    bool api_case_special();
    bool execute_class_performance_case();
    bool execute_class_stress_case();
    bool execute_recursive(String string_data);
    static int recursive_number;
};

#endif // STRINGTEST_HPP
