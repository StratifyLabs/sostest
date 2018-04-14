#ifndef CHECKSUMTEST_HPP
#define CHECKSUMTEST_HPP

#include <sapi/var.hpp>
#include <sapi/test.hpp>

class ChecksumTest : public Test {
public:
    ChecksumTest();


private:
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();
};

#endif // CHECKSUMTEST_HPP
