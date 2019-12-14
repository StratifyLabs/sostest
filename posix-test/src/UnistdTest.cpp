#include "UnistdTest.hpp"

UnistdTest::UnistdTest() : Test("UnistdTest"){}


bool UnistdTest::execute_class_api_case(){
   {
      Function<int, const char *, int> test("access", access, this);
      test.expect_result(
               0,
               "/",
               R_OK);

      test.expect_error(
               ENAMETOOLONG,
               "/0123456789/0123456789/0123456789/0123456789/0123456789/0123456789/0123456789/0123456789/0123456789/",
               R_OK);
   }

   return case_result();
}
