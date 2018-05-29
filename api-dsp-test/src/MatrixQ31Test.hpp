#ifndef MATRIXQ31TEST_HPP
#define MATRIXQ31TEST_HPP

#include <sapi/test.hpp>

class MatrixQ31Test : public Test {
public:
    MatrixQ31Test();


    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();
};

#endif // MATRIXQ31TEST_HPP
