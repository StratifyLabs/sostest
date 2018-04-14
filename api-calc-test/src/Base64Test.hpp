#ifndef BASE64TEST_HPP
#define BASE64TEST_HPP

#include <sapi/var.hpp>
#include <sapi/test.hpp>

class Base64Test : public Test {
public:
    Base64Test();


private:
    bool execute_class_api_case();
    bool execute_class_performance_case();
    bool execute_class_stress_case();
};

#endif // BASE64TEST_HPP
