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
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <dirent.h>

#include "tests.h"

#define NUM_DIRECTORIES 3
#define NUM_SUBDIRECTORIES 3
#define NUM_FILES 3

static int mkdir_test(void);
static int rmdir_test(void);

static int create_files(const char * dir_path, int num_files);
static int read_check_files(const char * dir_path);
static int remove_files(const char * dir_path);

int unistd_directory_test(void){

	if ( mkdir_test() < 0 ){
		return -1;
	}

	if ( rmdir_test() < 0 ){
		return -1;
	}

	return 0;

}

int mkdir_test(void){
	char path[PATH_MAX];
	int i;
	int j;

	//Create and remove some directories
	printf("Test mkdir() 0777...");
	fflush(stdout);
	for(i = 0; i < NUM_DIRECTORIES; i++){
		printf(".");
		fflush(stdout);
		sprintf(path, "/home/dir%d", i);
		if ( mkdir(path, 0777 ) < 0){

			if ( errno == ENOTSUP ){
				printf("Not Supported...abort\n");
				return 0;
			}

			if ( errno != EEXIST ){
				fflush(stdout);
				perror("failed");
				return -1;
			}
		}


		for(j = 0; j < NUM_SUBDIRECTORIES; j++){
			printf(".");
			fflush(stdout);
			sprintf(path, "/home/dir%d/dir%d", i, j);
			if ( mkdir(path, 0777 ) ){
				if ( errno != EEXIST ){
					fflush(stdout);
					perror("failed");
					return -1;
				}
			}
			if ( create_files(path, NUM_FILES) < 0 ){
				return -1;
			}
		}

	}

	printf("passed\n");

	printf("Test mkdir() 0777...");
	if ( mkdir("/home/nosearch", 0777 ) ){ //not searchable
		if ( errno != EEXIST ){
			fflush(stdout);
			perror("failed");
			return -1;
		}
	}

	if ( mkdir("/home/nosearch/test", 0777 ) ){ // /home/nosearch is not searchable so this should fail
		if ( errno != EEXIST ){
			fflush(stdout);
			perror("failed");
			return -1;
		}
	}

	if ( chmod("/home/nosearch", 0666) ){
		fflush(stdout);
		perror("chmod() failed");
		return -1;
	}

	if ( mkdir("/home/nosearch/test1", 0777 ) ){ // /home/nosearch is not searchable so this should fail
		if ( errno != EACCES ){
			printf("Failed to detect permissions error in creating /home/nosearch/test (%d)\n", errno);
			return -1;
		}
	}
	printf("passed\n");

	return 0;
}

int rmdir_test(void){
	int i;
	int j;
	char path[PATH_MAX];

	printf("Test rmdir() (not empty)...");
	if ( rmdir("/home/dir0") == 0 ){
		printf("Should not have deleted /home/dir0 because it's not empty\n");
		return -1;
	}

	if( errno == ENOTSUP ){
		printf("Not Supported...abort\n");
		return 0;
	}

	errno = 0;
	printf("passed\n");

	printf("Test rmdir()...");
	fflush(stdout);
	for(i = 0; i < NUM_DIRECTORIES; i++){
		for(j = 0; j < NUM_SUBDIRECTORIES; j++){
			printf(".");
			fflush(stdout);
			sprintf(path, "/home/dir%d/dir%d", i, j);

			if ( read_check_files(path) < 0 ){
				return -1;
			}

			if ( remove_files(path) < 0 ){
				return -1;
			}

			if ( rmdir(path) ){
				if ( errno != EEXIST ){
					fflush(stdout);
					perror("failed");
					return -1;
				}
			}
		}

		printf(".");
		fflush(stdout);
		sprintf(path, "/home/dir%d", i);
		if ( rmdir(path) ){
			if ( errno != EEXIST ){
				fflush(stdout);
				perror("failed");
				return -1;
			}
		}
	}

	printf("passed\n");

	if ( chmod("/home/nosearch", 0777) ){
		fflush(stdout);
		perror("chmod() failed");
		return -1;
	}

	printf("Test rmdir()...");
	if ( rmdir("/home/nosearch/test") ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	printf("Test rmdir()...");
	if ( rmdir("/home/nosearch") ){
		fflush(stdout);
		perror("failed");
		return -1;
	}
	printf("passed\n");

	return 0;
}

int create_files(const char * dir_path, int num_files){
	char buffer[PATH_MAX];
	int i;
	FILE * f;

	for(i = 0; i < num_files; i++){
		sprintf(buffer, "%s/test%d.txt", dir_path, i);
		f = fopen(buffer, "w");
		if ( f == NULL ){
			sprintf(buffer, "Failed to create %s/test%d.txt", dir_path, i);
			perror(buffer);
			return -1;
		}

		strcpy(buffer, "This is a write test\n");
		if ( fwrite(buffer, strlen(buffer), 1, f) != 1 ){
			sprintf(buffer, "Failed to write %s/test%d.txt", dir_path, i);
			perror(buffer);
			fclose(f);
			return -1;
		}
		fclose(f);
	}
	return 0;
}

int remove_files(const char * dir_path){
	char buffer[PATH_MAX];
	int i;

	for(i = 0; i < NUM_FILES; i++){
		sprintf(buffer, "%s/test%d.txt", dir_path, i);
		if( remove(buffer) < 0 ){
			sprintf(buffer, "Failed to remove %s/test%d.txt", dir_path, i);
			perror(buffer);
			return -1;
		}
	}
	return 0;
}

int read_check_files(const char * dir_path){
	char buffer[PATH_MAX];
	int i;
	FILE * f;

	for(i = 0; i < NUM_FILES; i++){
		sprintf(buffer, "%s/test%d.txt", dir_path, i);
		f = fopen(buffer, "r");
		if ( f == NULL ){
			sprintf(buffer, "Failed to create %s/test%d.txt", dir_path, i);
			perror(buffer);
			return -1;
		}

		if ( fgets(buffer, PATH_MAX, f) < 0 ){
			sprintf(buffer, "Failed to read %s/test%d.txt", dir_path, i);
			perror(buffer);
			fclose(f);
			return -1;
		}

		if (strcmp(buffer, "This is a write test\n") ){
			printf("Read test failed for %s/test%d.txt: %s\n", dir_path, i, buffer);
			return -1;
		}
		fclose(f);
	}
	return 0;
}
