#include <stdio.h>

#include <sapi/sys.hpp>
#include <sapi/test.hpp>

#include "ThreadTest.hpp"


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
    FILE_INFO_TEST_FLAG = (1<<10),
};

u32 decode_cli(const Cli & cli, u32 & execute_flags);

int main(int argc, char * argv[]){
    Cli cli(argc, argv);
    cli.set_publisher("Stratify Labs, Inc");
    cli.handle_version();
    u32 o_flags;
    u32 o_execute_flags;

    o_flags = decode_cli(cli, o_execute_flags);

    Test::initialize(cli.name(), cli.version());

    if( o_flags & THREAD_TEST_FLAG ){
        ThreadTest test;
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

    return o_flags;

}



