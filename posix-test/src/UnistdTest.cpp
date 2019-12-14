#include "UnistdTest.hpp"
#include <unistd.h>

#define EXEC_PATH "/app/flash/posix"

UnistdTest::UnistdTest() : Test("UnistdTest"){}


bool UnistdTest::execute_class_api_case(){

   execute_api_access_case();


   return case_result();
}


bool UnistdTest::execute_api_access_case(){
	int fd;

	TEST_THIS_EXPECT(int, access(EXEC_PATH, F_OK), 0);
	TEST_THIS_EXPECT_ERROR(ENOENT, access("/nomount/some/file", F_OK));
	TEST_THIS_EXPECT_ERROR(ENOENT, access("/nomount", F_OK));
	TEST_THIS_EXPECT_ERROR(ENAMETOOLONG, access("/0123456789012345678901234567890123456789012345678901234567890123456789", F_OK));

	TEST_THIS_EXPECT_ERROR(ENAMETOOLONG, access("/0123456789/01234567890/123456789/0123/4567890/123456789012345/67890123/456/789", F_OK));

	//create a file in home
	TEST_THIS_EXPECT(bool, (fd = open("/home/test.txt", O_RDWR | O_CREAT, 0666)) >= 0, true);
	TEST_THIS_EXPECT(bool, write(fd, "test", 4) < 0, false);
	TEST_THIS_EXPECT(bool, close(fd) < 0, false);

	TEST_THIS_EXPECT(int, access("/home/test.txt", W_OK), 0);
	TEST_THIS_EXPECT_ERROR(EACCES, access(EXEC_PATH, W_OK));
	TEST_THIS_EXPECT_ERROR(EACCES, access("/", W_OK));
	TEST_THIS_EXPECT(int, access(EXEC_PATH, R_OK), 0);

	TEST_THIS_EXPECT_ERROR(EACCES, access("/app/.install", R_OK));
	TEST_THIS_EXPECT(int, access(EXEC_PATH, X_OK), 0);
	TEST_THIS_EXPECT_ERROR(EACCES, access("/app/.install", X_OK));

	return case_result();
}

bool UnistdTest::execute_api_sleep_case(){

}

bool UnistdTest::execute_api_directory_case(){

	//ENOENT
	//ENAMETOOLONG
	//ENOTSUP
	//EINVAL


	return case_result();
}

bool UnistdTest::execute_api_file_case(){

	return case_result();
}
