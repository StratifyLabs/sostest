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

#include <stdio.h>
#include <string.h>
#include <unistd.h>


int stdio_test(){
	char buffer[64];

	printf("Test STDIO (stdout)...");
	printf("passed\n");

	printf("Test STDIO (stdin) Enter HELLO...");
	fflush(stdout);
	fflush(stdin);
	fgets(buffer, 64, stdin);
	if( strcmp(buffer, "HELLO\n") == 0 ){
		printf("passed\n");
	} else {
		printf("failed (did you enter \"HELLO\"?)\n");

		printf("-%s- was received\n", buffer);

		printf("stdio fd %d %d\n", stdout->_file, stdin->_file);

		return -1;
	}

	fflush(stdin);
	printf("Test STDIO (stdin) Enter HELLO...");
	fflush(stdout);
	fgets(buffer, 3, stdin);
	if( strcmp(buffer, "HE") == 0 ){
		printf("passed\n");
	} else {
		printf("failed (did you enter \"HELLO\"?)\n");
		return -1;
	}


	return 0;
}
