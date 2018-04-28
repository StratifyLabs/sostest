/*
Copyright 2016 Tyler Gilbert

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sapi/sys.hpp>
#include <sapi/test.hpp>
#include "tests.h"

#include "SchedTest.hpp"

enum {
    STDIO_TEST = 1<<4,
    AIO_TEST = 1<<5,
    SEM_TEST = 1<<6,
    LISTIO_TEST = 1<<7,
    DIRENT_TEST = 1<<8,
    MQ_TEST = 1<<9,
    SCHED_TEST = 1<<10,
    PTHREAD_TEST = 1<<11,
    DIRECTORY_TEST = 1<<12,
    FILE_TEST = 1<<13,
    ACCESS_TEST = 1<<14,
    SLEEP_TEST = 1<<15,
    SIGNAL_MASTER_TEST = 1<<16,
    LAUNCH_TEST = 1<<17
};

static u32 decode_cli(const Cli & cli);
static void show_usage(const Cli & cli);

int main(int argc, char * argv[]){
    Cli cli(argc, argv);
    cli.handle_version();
    u32 o_execute_flags;


    o_execute_flags = decode_cli(cli);


    if( o_execute_flags == 0 ){
        show_usage(cli);
        exit(1);
    }

    Test::initialize("posix", cli.version());

    if( o_execute_flags & SCHED_TEST ){
        SchedTest test;
        test.execute(o_execute_flags);
    }

    Test::finalize();

#if 0

    if( o_execute_flags & STDIO_TEST ){
        if( stdio_test() < 0 ){
            printf("STDIO Test Failed\n");
            return -1;
        }
    }

    if( o_execute_flags & AIO_TEST ){
        if ( aio_test() < 0 ){
            printf("AIO Test Failed\n");
            return -1;
        }
    }


    if( o_execute_flags & LISTIO_TEST ){
        if ( listio_test() < 0 ){
            printf("LIO Test Failed\n");
            return -1;
        }
    }

    if( o_execute_flags & DIRENT_TEST ){
        if ( dirent_test() < 0 ){
            printf("DIRENT Test Failed\n");
            return -1;
        }
    }

    if( o_execute_flags & SEM_TEST ){
        if ( sem_test() < 0 ){
            printf("SEM Test Failed\n");
            return -1;
        }
    }

    if( o_execute_flags & MQ_TEST ){
        if ( mqueue_test() < 0 ){
            printf("MQUEUE Test Failed\n");
            return -1;
        }
    }

    if( o_execute_flags & NUM_TEST ){
        if ( num_test() < 0 ){
            printf("NUM Test Failed\n");
            return -1;
        }
    }

    if( o_execute_flags & SCHED_TEST ){
        if ( sched_test() < 0 ){
            printf("SCHED Test Failed\n");
            return -1;
        }
    }

    if( o_execute_flags & PTHREAD_TEST ){
        if ( pthread_master_test() < 0 ){
            printf("PTHREAD Test Failed\n");
            return -1;
        }
    }

    if( o_execute_flags & DIRECTORY_TEST ){
        if ( unistd_directory_test() < 0 ){
            printf("Directory test failed\n");
            exit(1);
        }
    }

    if( o_execute_flags & FILE_TEST ){
        if ( unistd_file_test() < 0 ){
            printf("File test failed\n");
            exit(1);
        }
    }

    if( o_execute_flags & ACCESS_TEST ){
        if( unistd_access_test() < 0 ){
            printf("Access test failed\n");
            exit(1);
        }
    }

    if( o_execute_flags & SLEEP_TEST ){
        if ( unistd_sleep_test() < 0 ){
            printf("Sleep test failed\n");
            exit(1);
        }
    }

    if( o_execute_flags & SIGNAL_MASTER_TEST ){
        if ( signal_master_test() < 0 ){
            printf("SIGNAL Master Test Failed\n");
            return -1;
        }
    }

    if( o_execute_flags & LAUNCH_TEST ){
        if( launch_test() < 0 ){
            printf("LAUNCH Test Failed\n");
            return -1;
        }
    }
#endif

    return 0;


}
u32 decode_cli(const Cli & cli){
    u32 o_flags = 0;

    if(cli.is_option("-all") ){
        o_flags = 0xffffffff;
        return o_flags;
    }

    if(cli.is_option("-execute_all")){ o_flags |= Test::EXECUTE_ALL; }
    if(cli.is_option("-api") ){ o_flags |= Test::EXECUTE_API; }
    if(cli.is_option("-performance") ){ o_flags |= Test::EXECUTE_PERFORMANCE; }
    if(cli.is_option("-stress") ){ o_flags |= Test::EXECUTE_STRESS; }

    if(cli.is_option("-test_all") ){ o_flags |= 0xfffffff0; }
    if(cli.is_option("-stdio") ){ o_flags |= STDIO_TEST; }
    if(cli.is_option("-aio") ){ o_flags |= AIO_TEST; }
    if(cli.is_option("-sem") ){ o_flags |= SEM_TEST; }
    if(cli.is_option("-listio") ){ o_flags |= LISTIO_TEST; }
    if(cli.is_option("-dirent") ){ o_flags |= DIRENT_TEST; }
    if(cli.is_option("-mq") )  { o_flags |= MQ_TEST; }
    if(cli.is_option("-sched") ){ o_flags |= SCHED_TEST; }
    if(cli.is_option("-pthread") ){ o_flags |= PTHREAD_TEST; }
    if(cli.is_option("-directory") ){ o_flags |= DIRECTORY_TEST; }
    if(cli.is_option("-file") ){ o_flags |= FILE_TEST; }
    if(cli.is_option("-access") ){ o_flags |= ACCESS_TEST; }
    if(cli.is_option("-sleep") ){ o_flags |= SLEEP_TEST; }
    if(cli.is_option("-signal") ){ o_flags |= SIGNAL_MASTER_TEST; }
    if(cli.is_option("-launch") ){ o_flags |= LAUNCH_TEST; }

    return o_flags;

}

void show_usage(const Cli & cli){
    printf("\n");
    printf("usage: %s\n", cli.name());
    printf("    -all            execute all tests and all test types.\n");
//test type
    printf("    -execute_all    execute API, performance, and stress tests.\n");
    printf("    -api            execute API tests.\n");
    printf("    -performance    execute performance tests.\n");
    printf("    -stress         execute stress tests.\n");
//data type
    printf("    -test_all       execute all POSIX tests.\n");
    printf("    -stdio          execute stdio tests (requires user interaction).\n");
    printf("    -aio            execute aio tests.\n");
    printf("    -sem            execute semaphore tests.\n");
    printf("    -listio         execute listio tests.\n");
    printf("    -dirent         execute dirent tests.\n");
    printf("    -mq             execute message queue tests.\n");
    printf("    -sched          execute scheduler tests.\n");
    printf("    -pthread        execute POSIX threads tests.\n");
    printf("    -directory      execute directory tests.\n");
    printf("    -file           execute file tests.\n");
    printf("    -access         execute access tests.\n");
    printf("    -sleep          execute sleep and usleep tests.\n");
    printf("    -signal         execute signal tests.\n");
    printf("    -launch         execute launch tests (launches other applications - non POSIX).\n");
}
