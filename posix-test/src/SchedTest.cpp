#include "SchedTest.hpp"

#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include <sched.h>
#include <unistd.h>

SchedTest::SchedTest() : Test("posix::sched"){

}


bool SchedTest::execute_class_api_case(){
   bool result = true;
   struct sched_param param;
   int max_prio;
   int min_prio;
   int i;


   {
      Function<int, pid_t, struct sched_param*> test("sched_getparam", sched_getparam, this);
      test.expect_error(ESRCH,-1,&param);
      test.expect_result(0,getpid(),&param);

      TEST_EXPECT(test, int, param.sched_priority, 0);

   }

   {
      Function<int, pid_t, const struct sched_param*> test("sched_setparam", sched_setparam, this);
      test.execute_case_with_less_than_zero_on_error("set param ESRCH", ESRCH, -1, &param);
      param.sched_priority = 100;
      test.execute_case_with_less_than_zero_on_error("set param EINVAL", EINVAL, getpid(), &param);
      param.sched_priority = 0;
      test.execute_case_with_less_than_zero_on_error("set param SUCCESS", 0, getpid(), &param);
   }


   {
      min_prio = sched_get_priority_min(SCHED_FIFO);
      max_prio = sched_get_priority_max(SCHED_FIFO);
      String name;
      Function<int, pid_t, int, const struct sched_param*> test("sched_setscheduler", sched_setscheduler, this);
      for(i = min_prio; i <= max_prio; i++){
         wait(Microseconds(1000));
         param.sched_priority = i;
         name.format("set scheduler SCHED_FIFO:%d", i);
         test.execute_case_with_less_than_zero_on_error(name.cstring(), 0, getpid(), SCHED_FIFO, &param);
      }

      min_prio = sched_get_priority_min(SCHED_FIFO);
      max_prio = sched_get_priority_max(SCHED_FIFO);
      for(i = min_prio; i <= max_prio; i++){
         wait(Microseconds(1000));
         param.sched_priority = i;
         name.sprintf("set scheduler SCHED_RR:%d", i);
         test.execute_case_with_less_than_zero_on_error(name.cstring(), 0, getpid(), SCHED_RR, &param);
      }

      min_prio = sched_get_priority_min(SCHED_OTHER);
      max_prio = sched_get_priority_max(SCHED_OTHER);
      for(i = min_prio; i <= max_prio; i++){
         wait(Microseconds(1000));
         param.sched_priority = i;
         name.sprintf("set scheduler SCHED_OTHER:%d", i);
         test.execute_case_with_less_than_zero_on_error(name.cstring(), 0, getpid(), SCHED_OTHER, &param);
      }
   }

   return result;
}
