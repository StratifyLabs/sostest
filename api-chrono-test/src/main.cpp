#include <stdio.h>

#include <sapi/sys.hpp>
#include <sapi/test.hpp>

#include "TimerTest.hpp"
#include "MicroTimeTest.hpp"
#include "TimeTest.hpp"

//update flags
enum {
    //Test flags occupy lower bit values
    MICRO_TIME_TEST_FLAG = (1<<5),
    TIMER_TEST_FLAG = (1<<6),
    TIME_TEST_FLAG = (1<<7)
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

    if( o_flags & TIMER_TEST_FLAG ){
        TimerTest test;
        test.execute(o_execute_flags);
    }

    if( o_flags & MICRO_TIME_TEST_FLAG ){
        MicroTimeTest test;
        test.execute(o_execute_flags);
    }

    if( o_flags & TIME_TEST_FLAG ){
        TimeTest test;
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
    if(cli.is_option("-test_all") ){ o_flags = 0xffffffff; }
    if(cli.is_option("-micro_time") ){ o_flags |= MICRO_TIME_TEST_FLAG; }
    if(cli.is_option("-timer") ){ o_flags |= TIMER_TEST_FLAG; }
    if(cli.is_option("-time") ){ o_flags |= TIME_TEST_FLAG; }

    return o_flags;

}

void show_usage(const Cli & cli){
    printf("\n");
    printf("usage: %s\n", cli.name());
    printf("    -all            execute all type of test for all object.\n");
    printf("    -execute_all    execute all type of test.\n");
    printf("    -api            execute api test.\n");
    printf("    -stress         execute stress test.\n");
    printf("    -performance    execute performance test.\n");
    printf("    -additional     execute additional test.\n");

    printf("    -test_all       execute test for all object.\n");
    printf("    -micro_time     execute test for chrono/MicroTime.\n");
    printf("    -timer          execute test for chrono/Timer.\n");
    printf("    -time           execute test for sys/Time.\n");
}



