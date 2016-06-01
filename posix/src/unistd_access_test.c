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
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdbool.h>

#include "tests.h"

#define EXEC_PATH "/app/flash/posix"

int unistd_access_test(){
	//check the access to the executable
	int fd;
	int ret;

	test_intro("access", 0, "/app/flash/posix, F_OK");
	ret = access(EXEC_PATH, F_OK);
	if( test_success(ret) < 0 ){ return -1; }

	test_intro("access", "ENOENT", "/nomount/some/file, F_OK");
	ret = access("/nomount/some/file", F_OK);
	if( test_errno(ret, "ENOENT", ENOENT) < 0 ){ return -1; }

	test_intro("access", "ENOENT", "/nomount, F_OK");
	ret = access("/nomount", F_OK);
	if( test_errno(ret, "ENOENT", ENOENT) < 0 ){ return -1; }

	test_intro("access", "ENAMETOOLONG", "/0123456789012345678901234567890123456789012345678901234567890123456789, F_OK");
	ret = access("/0123456789012345678901234567890123456789012345678901234567890123456789", F_OK);
	if( test_errno(ret, "ENAMETOOLONG", ENAMETOOLONG) < 0 ){ return -1; }

	test_intro("access", "ENAMETOOLONG", "/0123456789/01234567890/123456789/0123/4567890/123456789012345/67890123/456/789, F_OK");
	ret = access("/0123456789/01234567890/123456789/0123/4567890/123456789012345/67890123/456/789", F_OK);
	if( test_errno(ret, "ENAMETOOLONG", ENAMETOOLONG) < 0 ){ return -1; }

	//create a file in home
	test_intro("access", 0, "/home/test.txt, W_OK");
	fd = open("/home/test.txt", O_RDWR | O_CREAT, 0666);
	if( fd < 0 ){
		return test_failed_perror();
	}
	if( write(fd, "test", 4) < 0 ){
		close(fd);
		return test_failed_perror();
	}
	if( close(fd) < 0 ){
		return test_failed_perror();
	}
	ret = access("/home/test.txt", W_OK);
	if( test_success(ret) < 0 ){ return -1; }

	test_intro("access", "EACCESS", "app/flash/posix, W_OK");
	ret = access(EXEC_PATH, W_OK);
	if( test_errno(ret, "EACCES", EACCES) < 0 ){ return -1; }


	test_intro("access", "EACCESS", "/, W_OK");
	ret = access("/", W_OK);
	if( test_errno(ret, "EACCES", EACCES) < 0 ){ return -1; }

	test_intro("access", 0, "/app/flash/posix, R_OK");
	ret = access(EXEC_PATH, R_OK);
	if( test_success(ret) < 0 ){ return -1; }


	test_intro("access", "EACCES", "/app/.install, R_OK");
	ret = access("/app/.install", R_OK);
	if( test_errno(ret, "EACCES", EACCES) < 0 ){ return -1; }


	test_intro("access", 0, "/app/flash/posix, X_OK");
	ret = access(EXEC_PATH, X_OK);
	if( test_success(ret) < 0 ){ return -1; }

	test_intro("access", "EACCES", "/app/.install, X_OK");
	ret = access("/app/.install", X_OK);
	if( test_errno(ret, "EACCES", EACCES) < 0 ){ return -1; }


	return 0;
}


