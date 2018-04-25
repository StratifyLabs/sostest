#include <stdio.h>

#include <sapi/sys.hpp>
#include <sapi/test.hpp>

#include "Base64Test.hpp"
#include "ChecksumTest.hpp"
#include "EmaTest.hpp"
#include "LookupTest.hpp"
#include "PidTest.hpp"
#include "RleTest.hpp"

enum {
    BASE64_TEST_FLAG = (1<<5),
    CHECKSUM_TEST_FLAG = (1<<6),
    EMA_TEST_FLAG = (1<<7),
    LOOKUP_TEST_FLAG = (1<<8),
    PID_TEST_FLAG = (1<<9),
    RLE_TEST_FLAG = (1<<10),
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

    if( o_flags & BASE64_TEST_FLAG){
        Base64Test test;
        test.execute(o_execute_flags);
    }

    if( o_flags & CHECKSUM_TEST_FLAG){
        ChecksumTest test;
        test.execute(o_execute_flags);
    }

    if( o_flags & EMA_TEST_FLAG){
        EmaTest test;
        test.execute(o_execute_flags);
    }

    if( o_flags & LOOKUP_TEST_FLAG){
        LookupTest test;
        test.execute(o_execute_flags);
    }

    if( o_flags & PID_TEST_FLAG){
        PidTest test;
        test.execute(o_execute_flags);
    }

    if( o_flags & RLE_TEST_FLAG){
        RleTest test;
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
    if(cli.is_option("-base64") ){ o_flags |= BASE64_TEST_FLAG; }
    if(cli.is_option("-checksum") ){ o_flags |= CHECKSUM_TEST_FLAG; }
    if(cli.is_option("-ema") ){ o_flags |= EMA_TEST_FLAG; }
    if(cli.is_option("-lookup") ){ o_flags |= LOOKUP_TEST_FLAG; }
    if(cli.is_option("-pid") ){ o_flags |= PID_TEST_FLAG; }
    if(cli.is_option("-rle") ){ o_flags |= RLE_TEST_FLAG; }

    return o_flags;

}



