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
#include "tests.h"


#define STDIO_TEST 0
#define AIO_TEST 0
#define SEM_TEST 1
#define LISTIO_TEST 0
#define DIRENT_TEST 1
#define MQ_TEST 1
#define NUM_TEST 0
#define SCHED_TEST 1
#define PTHREAD_TEST 1
#define DIRECTORY_TEST 1
#define FILE_TEST 0
#define ACCESS_TEST 1
#define SLEEP_TEST 0
#define SIGNAL_MASTER_TEST 1
#define LAUNCH_TEST 1

int main(int argc, char * argv[]){

	if( STDIO_TEST ){
		if( stdio_test() < 0 ){
			printf("STDIO Test Failed\n");
			return -1;
		}
	}

	if( AIO_TEST ){
		if ( aio_test() < 0 ){
			printf("AIO Test Failed\n");
			return -1;
		}
	}


	if( LISTIO_TEST ){
		if ( listio_test() < 0 ){
			printf("LIO Test Failed\n");
			return -1;
		}
	}

	if( DIRENT_TEST ){
		if ( dirent_test() < 0 ){
			printf("DIRENT Test Failed\n");
			return -1;
		}
	}

	if( SEM_TEST ){
		if ( sem_test() < 0 ){
			printf("SEM Test Failed\n");
			return -1;
		}
	}

	if( MQ_TEST ){
		if ( mqueue_test() < 0 ){
			printf("MQUEUE Test Failed\n");
			return -1;
		}
	}

	if( NUM_TEST ){
		if ( num_test() < 0 ){
			printf("NUM Test Failed\n");
			return -1;
		}
	}

	if( SCHED_TEST ){
		if ( sched_test() < 0 ){
			printf("SCHED Test Failed\n");
			return -1;
		}
	}

	if( PTHREAD_TEST ){
		if ( pthread_master_test() < 0 ){
			printf("PTHREAD Test Failed\n");
			return -1;
		}
	}

	if ( DIRECTORY_TEST ){
		if ( unistd_directory_test() < 0 ){
			printf("Directory test failed\n");
			exit(1);
		}
	}

	if ( FILE_TEST ){
		if ( unistd_file_test() < 0 ){
			printf("File test failed\n");
			exit(1);
		}
	}

	if( ACCESS_TEST ){
		if( unistd_access_test() < 0 ){
			printf("Access test failed\n");
			exit(1);
		}
	}

	if ( SLEEP_TEST ){
		if ( unistd_sleep_test() < 0 ){
			printf("Sleep test failed\n");
			exit(1);
		}
	}

	if( SIGNAL_MASTER_TEST ){
		if ( signal_master_test() < 0 ){
			printf("SIGNAL Master Test Failed\n");
			return -1;
		}
	}

	if( LAUNCH_TEST ){
		if( launch_test() < 0 ){
			printf("LAUNCH Test Failed\n");
			return -1;
		}
	}

	printf("All Tests Passed\n");
	return 0;


}
