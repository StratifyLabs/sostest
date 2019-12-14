#ifndef AIO_TEST_HPP
#define AIO_TEST_HPP

#include <sapi/test.hpp>

class AioTest:public Test {
public:
   u32 wait_time = 4000;/*!<wait time for object thread*/
   AioTest();
private:
   //api
   bool execute_class_api_case();
   //stress
   bool execute_class_stress_case();
   //performance
   bool execute_class_performance_case();

};

#endif // AIO_TEST_HPP
