#ifndef FILTERQ15TEST_HPP
#define FILTERQ15TEST_HPP

#include <sapi/test.hpp>

class FilterQ15Test : public Test {
public:
    FilterQ15Test();

    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();
};

#endif // FILTERQ15TEST_HPP
