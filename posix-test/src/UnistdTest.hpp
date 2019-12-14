#ifndef UNISTDTEST_HPP
#define UNISTDTEST_HPP


#include <sapi/test.hpp>

class UnistdTest : public Test {
public:
   UnistdTest();

   bool execute_class_api_case();

private:
   bool execute_api_access_case();
   bool execute_api_sleep_case();
   bool execute_api_directory_case();
   bool execute_api_file_case();

};

#endif // UNISTDTEST_HPP
