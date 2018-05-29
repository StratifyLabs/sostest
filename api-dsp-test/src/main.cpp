#include <stdio.h>

#include <sapi/sys.hpp>
#include <sapi/test.hpp>

#include "TransformQ31Test.hpp"
#include "SignalQ31Test.hpp"
#include "MatrixQ31Test.hpp"
#include "FilterQ31Test.hpp"

//update flags
enum {
    SIGNAL_TEST_FLAG = (1<<5),
    TRANSFORM_TEST_FLAG = (1<<6),
    FILTER_TEST_FLAG = (1<<7),
    MATRIX_TEST_FLAG = (1<<8),
    Q7_TEST_FLAG = (1<<9),
    Q15_TEST_FLAG = (1<<10),
    Q31_TEST_FLAG = (1<<11),
    F32_TEST_FLAG = (1<<12),
};

static void show_usage(const Cli & cli);
static u32 decode_cli(const Cli & cli, u32 & execute_flags);

int main(int argc, char * argv[]){
    Cli cli(argc, argv);
    cli.set_publisher("Stratify Labs, Inc");
    cli.handle_version();
    u32 o_flags;
    u32 o_execute_flags;

    o_flags = decode_cli(cli, o_execute_flags);

    if( o_flags == 0 ){
        show_usage(cli);
        exit(1);
    }

    Test::initialize(cli.name(), cli.version());

    if( (o_flags & SIGNAL_TEST_FLAG) && (o_flags & Q31_TEST_FLAG) ){
        SignalQ31Test test;
        test.execute(o_execute_flags);
    }

    if( (o_flags & FILTER_TEST_FLAG) && (o_flags & Q31_TEST_FLAG) ){
        FilterQ31Test test;
        test.execute(o_execute_flags);
    }

    if( (o_flags & MATRIX_TEST_FLAG) && (o_flags & Q31_TEST_FLAG) ){
        MatrixQ31Test test;
        test.execute(o_execute_flags);
    }

    if( (o_flags & TRANSFORM_TEST_FLAG) && (o_flags & Q31_TEST_FLAG) ){
        TransformQ31Test test;
        test.execute(o_execute_flags);
    }

    Test::finalize();

    return 0;

}

void show_usage(const Cli & cli){
    printf("Usage: %s\n", cli.name());
    printf("TODO: Show usage\n");
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
    if(cli.is_option("-signal") ){ o_flags |= SIGNAL_TEST_FLAG; }
    if(cli.is_option("-transform") ){ o_flags |= TRANSFORM_TEST_FLAG; }
    if(cli.is_option("-filter") ){ o_flags |= FILTER_TEST_FLAG; }
    if(cli.is_option("-matrix") ){ o_flags |= MATRIX_TEST_FLAG; }
    if(cli.is_option("-q7") ){ o_flags |= Q7_TEST_FLAG; }
    if(cli.is_option("-q15") ){ o_flags |= Q15_TEST_FLAG; }
    if(cli.is_option("-q31") ){ o_flags |= Q31_TEST_FLAG; }
    if(cli.is_option("-f32") ){ o_flags |= F32_TEST_FLAG; }

    return o_flags;

}



