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
#include "MessengerTest.hpp"
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

u32 decode_cli(const Cli & cli, u32 & execute_flags);
void show_usage(const Cli & cli);

int main(int argc, char * argv[]){
    Cli cli(argc, argv);
    cli.set_publisher("Stratify Labs, Inc");
    cli.handle_version();
    u32 o_flags;
    u32 o_execute_flags;
    o_flags = decode_cli(cli, o_execute_flags);
    if( o_flags == 0 ){
       show_usage(cli);
       exit(0);
    }
    Test::initialize(cli.name(), cli.version());
    if( o_flags & SCHED_TEST_FLAG ){
        SchedTest test;
        test.execute(o_execute_flags);
    }
    if( o_flags & THREAD_TEST_FLAG ){
        ThreadTest test;
        test.execute(o_execute_flags);
    }
    if( o_flags & MUTEX_TEST_FLAG ){
        MutexTest test;
        test.execute(o_execute_flags);
    }
    if( o_flags & MQ_TEST_FLAG ){
        MqTest test;
        test.execute(o_execute_flags);
    }
    if( o_flags & SYS_TEST_FLAG ){
        SysTest test;
        test.execute(o_execute_flags);
    }
    if( o_flags & SIGNAL_TEST_FLAG){
        SignalTest test;
        test.execute(o_execute_flags);
    }

    if( o_flags & TASK_TEST_FLAG){
        TaskTest test;
        test.execute(o_execute_flags);
    }

    if( o_flags & CLI_TEST_FLAG){
        CliTest test;
        test.execute(o_execute_flags);
    }
    if( o_flags & MESSENGER_TEST_FLAG){
        MessengerTest test;
        test.execute(o_execute_flags);
    }
    if( o_flags & TRACE_TEST_FLAG){
        TraceTest test;
        test.execute(o_execute_flags);
    }
    if( o_flags & APPFS_TEST_FLAG){
        AppfsTest test;
        test.execute(o_execute_flags);
    }

    Test::finalize();
    return 0;
}

u32 decode_cli(const Cli & cli, u32 & execute_flags){
    u32 o_flags = 0;
    execute_flags = 0;
    if(cli.is_option("-all") ){
        o_flags = 0xffffffff;
        execute_flags |= Test::EXECUTE_ALL;
        return o_flags;
    }
    if(cli.is_option("-execute_all") ){ execute_flags |= Test::EXECUTE_ALL; }
    if(cli.is_option("-api") ){ execute_flags |= Test::EXECUTE_API; }
    if(cli.is_option("-stress") ){ execute_flags |= Test::EXECUTE_STRESS; }
    if(cli.is_option("-performance") ){ execute_flags |= Test::EXECUTE_PERFORMANCE; }
    if(cli.is_option("-additional") ){ execute_flags |= Test::EXECUTE_ADDITIONAL; }
    //update switches
    if(cli.is_option("-test_all") ){ o_flags = 0xffffffe0; }
    if(cli.is_option("-aio") ){ o_flags |= AIO_TEST_FLAG; }
    if(cli.is_option("-appfs") ){ o_flags |= APPFS_TEST_FLAG; }
    if(cli.is_option("-assets") ){ o_flags |= ASSETS_TEST_FLAG; }
    if(cli.is_option("-cli") ){ o_flags |= CLI_TEST_FLAG; }
    if(cli.is_option("-dir") ){ o_flags |= DIR_TEST_FLAG; }
    if(cli.is_option("-file") ){ o_flags |= FILE_TEST_FLAG; }
    if(cli.is_option("-file_info") ){ o_flags |= FILE_INFO_TEST_FLAG; }
    if(cli.is_option("-messenger") ){ o_flags |= MESSENGER_TEST_FLAG; }
    if(cli.is_option("-mq") ){ o_flags |= MQ_TEST_FLAG; }
    if(cli.is_option("-mutex") ){ o_flags |= MUTEX_TEST_FLAG; }
    if(cli.is_option("-sched") ){ o_flags |= SCHED_TEST_FLAG; }
    if(cli.is_option("-sem") ){ o_flags |= SEM_TEST_FLAG; }
    if(cli.is_option("-signal") ){ o_flags |= SIGNAL_TEST_FLAG; }
    if(cli.is_option("-sys") ){ o_flags |= SYS_TEST_FLAG; }
    if(cli.is_option("-thread") ){ o_flags |= THREAD_TEST_FLAG; }
    if(cli.is_option("-trace") ){ o_flags |= TRACE_TEST_FLAG; }
    if(cli.is_option("-task") ){ o_flags |= TASK_TEST_FLAG; }
    return o_flags;
}


void show_usage(const Cli & cli){
    printf("\n");
    printf("usage: %s\n", cli.name());
    printf("    -all            execute all test types (stress, api, performance) for all objects\n");
    printf("    -execute_all    execute all test types\n");
    printf("    -api            execute api test\n");
    printf("    -stress         execute stress test\n");
    printf("    -performance    execute performance test\n");
    printf("    -additional     execute additional tests (if any)\n");
    printf("    -test_all       execute test for all objects\n");
//    printf("    -aio           execute test \n");
    printf("    -appfs          execute test for sys/appfs \n");
//    printf("    -assets         execute test\n");
    printf("    -cli            execute test \n");
//    printf("    -dir         execute test \n");
//    printf("    -file           execute test \n");
    printf("    -file_info      execute test for sys/FileInfo \n");
    printf("    -messenger      execute test for sys/messenger\n");
    printf("    -mq             execute test for sys/mq \n");
    printf("    -mutex          execute test for sys/mutex \n");
    printf("    -sched          execute test for sys/sched \n");
//    printf("    -sem         execute test \n");
    printf("    -signal         execute test for sys/signal\n");
    printf("    -sys            execute test for sys/sys \n");
    printf("    -task           execute test for sys/task \n");
    printf("    -thread         execute test for sys/thread \n");
    printf("    -trace          execute test for sys/trace \n");
    printf("    -v              options to show the version\n");
}


