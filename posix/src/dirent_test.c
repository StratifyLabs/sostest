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


#include <dirent.h>
#include <stdio.h>
#include <errno.h>


int dirent_test(){
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


