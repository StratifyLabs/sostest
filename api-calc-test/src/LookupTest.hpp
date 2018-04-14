#ifndef LOOKUPTEST_HPP
#define LOOKUPTEST_HPP

#include <sapi/var.hpp>
#include <sapi/test.hpp>

class LookupTest : public Test {
public:
    LookupTest();


private:
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();
};

#endif // LOOKUPTEST_HPP
