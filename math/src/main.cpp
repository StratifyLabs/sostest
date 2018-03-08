#include <stdio.h>

#include <sapi/sys.hpp>

#include "num_test.h"


int main(int argc, char * argv[]){
    u32 o_flags = NUM_TEST_FLAG_ALL;

    Cli cli(argc, argv);


    cli.handle_version();

    return num_test_execute(o_flags);
}



