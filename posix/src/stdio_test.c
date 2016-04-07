/*
 * stdio.c
 *
 *  Created on: Nov 29, 2013
 *      Author: tgil
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>


int stdio_test(void){
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
