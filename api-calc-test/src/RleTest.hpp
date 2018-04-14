#ifndef RLETEST_HPP
#define RLETEST_HPP

#include <sapi/var.hpp>
#include <sapi/test.hpp>

class RleTest : public Test {
public:
    RleTest();


private:
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();
};

#endif // RLETEST_HPP
