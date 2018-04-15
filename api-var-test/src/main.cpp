#include <stdio.h>

#include <sapi/sys.hpp>
#include <sapi/test.hpp>

#include "DataTest.hpp"
#include "QueueTest.hpp"
#include "StringTest.hpp"
#include "TokenTest.hpp"
#include "VectorTest.hpp"

enum {
    DATA_TEST_FLAG = (1<<5),
    QUEUE_TEST_FLAG = (1<<6),
    STRING_TEST_FLAG = (1<<7),
    TOKEN_TEST_FLAG = (1<<8),
    VECTOR_TEST_FLAG = (1<<9),
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

    Test::initialize(cli.name(), cli.version());

    if( o_flags == 0 ){
       show_usage(cli);
    }

    if( o_flags & DATA_TEST_FLAG ){
        DataTest test;
        test.execute(o_execute_flags);
    }

    if( o_flags & QUEUE_TEST_FLAG ){
        QueueTest test;
        test.execute(o_execute_flags);
    }

    if( o_flags & STRING_TEST_FLAG ){
        StringTest test;
        test.execute(o_execute_flags);
    }

    if( o_flags & VECTOR_TEST_FLAG ){
        VectorTest test;
        test.execute(o_execute_flags);
    }

    if( o_flags & TOKEN_TEST_FLAG ){
        TokenTest test;
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

    if(cli.is_option("-test_all") ){ o_flags = 0xffffffff; }
    if(cli.is_option("-data") ){ o_flags |= DATA_TEST_FLAG; }
    if(cli.is_option("-queue") ){ o_flags |= QUEUE_TEST_FLAG; }
    if(cli.is_option("-string") ){ o_flags |= STRING_TEST_FLAG; }
    if(cli.is_option("-token") ){ o_flags |= TOKEN_TEST_FLAG; }
    if(cli.is_option("-vector") ){ o_flags |= VECTOR_TEST_FLAG; }

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
    printf("    -data           execute test for Data.\n");
    printf("    -queue          execute test for Queue.\n");
    printf("    -string         execute test for String.\n");
    printf("    -token          execute test for Token.\n");
    printf("    -vector         execute test for Vector.\n");
    printf("    -ring           execute test for Ring.\n");
    printf("    -v              options to show the version.\n");

}



