#ifndef VECTORTEST_HPP
#define VECTORTEST_HPP

#include <sapi/test.hpp>

class VectorTest : public Test {
public:
    VectorTest();

private:
    //cases
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();
};

#endif // VECTORTEST_HPP
