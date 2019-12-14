#include <stdio.h>

#include <sapi/sys.hpp>
#include <sapi/test.hpp>

#include "ThreadTest.hpp"
#include "SchedTest.hpp"
#include "MutexTest.hpp"
#include "MqTest.hpp"
#include "SysTest.hpp"
#include "SignalTest.hpp"
#include "TaskTest.hpp"
#include "CliTest.hpp"
#include "TraceTest.hpp"
#include "AppfsTest.hpp"
#include "DirTest.hpp"
#include "FileTest.hpp"
#include "FileInfoTest.hpp"
#include "AioTest.hpp"
//update flags
enum {
   //Test::API PERFORMANCE and STRESS are the first flags
   AIO_TEST_FLAG = (1<<5),
   APPFS_TEST_FLAG = (1<<6),
   ASSETS_TEST_FLAG = (1<<7),
   CLI_TEST_FLAG = (1<<8),
   DIR_TEST_FLAG = (1<<9),
   FILE_TEST_FLAG = (1<<10),
   MESSENGER_TEST_FLAG = (1<<11),
   MQ_TEST_FLAG = (1<<12),
   MUTEX_TEST_FLAG = (1<<13),
   SCHED_TEST_FLAG = (1<<14),
   SEM_TEST_FLAG = (1<<15),
   SIGNAL_TEST_FLAG = (1<<16),
   SYS_TEST_FLAG = (1<<17),
   THREAD_TEST_FLAG = (1<<18),
   TRACE_TEST_FLAG = (1<<19),
   FILE_INFO_TEST_FLAG = (1<<20),
   TASK_TEST_FLAG = (1<<21),
};

u32 decode_cli(const Cli & cli);
void show_usage(const Cli & cli);

int main(int argc, char * argv[]){
   Cli cli(argc, argv, SOS_GIT_HASH);
   cli.set_publisher("Stratify Labs, Inc");
   cli.handle_version();
   u32 o_flags;
   o_flags = decode_cli(cli);

   if( o_flags == 0 ){
      show_usage(cli);
      exit(0);
   }

   if( cli.get_option("help") == "true" ){
      show_usage(cli);
      exit(0);
   }

   Test::initialize(
            Test::Name(cli.name()),
            Test::Version(cli.version()),
            Test::GitHash(SOS_GIT_HASH)
            );

   if( o_flags & SCHED_TEST_FLAG ){
      SchedTest test;
      test.execute(o_flags);
   }
   if( o_flags & THREAD_TEST_FLAG ){
      ThreadTest test;
      test.execute(o_flags);
   }
   if( o_flags & MUTEX_TEST_FLAG ){
      MutexTest test;
      test.execute(o_flags);
   }
   if( o_flags & MQ_TEST_FLAG ){
      MqTest test;
      test.execute(o_flags);
   }
   if( o_flags & SYS_TEST_FLAG ){
      SysTest test;
      test.execute(o_flags);
   }
   if( o_flags & SIGNAL_TEST_FLAG){
      SignalTest test;
      test.execute(o_flags);
   }

   if( o_flags & TASK_TEST_FLAG){
      TaskTest test;
      test.execute(o_flags);
   }

   if( o_flags & FILE_INFO_TEST_FLAG){
      FileInfoTest test;
      test.execute(o_flags);
   }

   if( o_flags & CLI_TEST_FLAG){
      CliTest test;
      test.execute(o_flags);
   }

   if( o_flags & TRACE_TEST_FLAG){
      TraceTest test;
      test.execute(o_flags);
   }
   if( o_flags & APPFS_TEST_FLAG){
      AppfsTest test;
      test.execute(o_flags);
   }

   Test::finalize();
   return 0;
}

u32 decode_cli(const Cli & cli){
   u32 o_flags = 0;
   o_flags = Test::parse_execution_flags(cli);
   o_flags |= Test::parse_test(cli, "aio", AIO_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "appfs", APPFS_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "assets", ASSETS_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "cli", CLI_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "dir", DIR_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "file", FILE_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "fileInfo", FILE_INFO_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "mq", MQ_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "mutex", MUTEX_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "sched", SCHED_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "sem", SEM_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "signal", SIGNAL_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "thread", THREAD_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "trace", TRACE_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "task", TASK_TEST_FLAG);
   return o_flags;
}


void show_usage(const Cli & cli){
   cli.show_options();
}


