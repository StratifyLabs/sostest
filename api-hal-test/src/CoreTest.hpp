#ifndef CORETEST_HPP
#define CORETEST_HPP
#include <sapi/hal.hpp>
#include <sapi/var.hpp>
#include <sapi/test.hpp>

class CoreTest : public Test {
public:
    CoreTest();

    bool execute_class_api_case();

    //Core does not need performance or stress classes
};

#endif // CORETEST_HPP
