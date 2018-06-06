#ifndef FILTERQ31_HPP
#define FILTERQ31_HPP

#include <sapi/test.hpp>

class FilterQ31Test : public Test {
public:
    FilterQ31Test();

    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();
};

#endif // FILTERQ31_HPP
