/*
 * dirent_test.c
 *
 *  Created on: May 25, 2013
 *      Author: tgil
 */


#include <dirent.h>
#include <stdio.h>
#include <errno.h>


int dirent_test(void){
	DIR * dirp;
	struct dirent * direntp;

	printf("Test opendir()...");
	dirp = opendir("/");
	if ( dirp == NULL ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	printf("Test readdir()...");
	do {
		errno = 0;
		direntp = readdir(dirp);
		if ( direntp == NULL ){
			if ( errno != 0 ){
				fflush(stdout);
				perror("failed");
				return -1;
			}
		} else {
			printf("%s...", direntp->d_name);
			fflush(stdout);
		}
	} while(direntp != NULL);

	printf("passed\n");

	printf("Test closedir()...");
	if( closedir(dirp) == 0 ){
		printf("passed\n");
	} else {
		fflush(stdout);
		perror("failed");
		return -1;
	}

	printf("Test readdir() (EBADF)...");
	if ( readdir(NULL) != NULL ){
		printf("failed\n");
		return -1;
	} else {
		if ( errno != EBADF ){
			fflush(stdout);
			perror("should have failed with EBADF");
			return -1;
		}
	}
	printf("passed\n");

	printf("Test closedir() (EBADF)...");
	if ( closedir(NULL) == 0 ){
		printf("failed\n");
		return -1;
	} else {
		if ( errno != EBADF ){
			fflush(stdout);
			perror("should have failed with EBADF");
			return -1;
		}
	}
	printf("passed\n");

	printf("Test opendir() (ENOENT)...");
	if ( opendir("/nodir") != NULL ){
		printf("failed\n");
		return -1;
	} else {
		if ( errno != ENOENT ){
			fflush(stdout);
			perror("should have failed with ENOENT");
			return -1;
		}
	}
	printf("passed\n");

	printf("Test opendir() (ENOENT)...");
	if ( opendir("/dev/no_dev") != NULL ){
		printf("failed\n");
		return -1;
	} else {
		if ( errno != ENOENT ){
			fflush(stdout);
			perror("should have failed with ENOENT");
			return -1;
		}
	}
	printf("passed\n");

	printf("Test opendir() (ENAMETOOLONG)...");
	if ( opendir("/usr/libs0123456789012345678901234567890123456789012345678901234567890123456789") != NULL ){
		printf("failed\n");
		return -1;
	} else {
		if ( errno != ENAMETOOLONG ){
			fflush(stdout);
			perror("should have failed with ENAMETOOLONG");
			return -1;
		}
	}
	printf("passed\n");



	return 0;
}


