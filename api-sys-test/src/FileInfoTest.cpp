#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include <sapi/fs.hpp>
#include "FileInfoTest.hpp"


FileInfoTest::FileInfoTest() : Test("sys::FileInfo"){
   Data::reclaim_heap_space();
}
/*@brief api test for sys/FileInfo use "api-sys-test -file_info -api"
 */
bool FileInfoTest::execute_class_api_case(){
   bool result = true;

   FileInfo file_info;

   file_info = File::get_info("/app/flash/api-sys-test");
   if( file_info.is_valid() == false ){
      file_info = File::get_info("/app/ram/api-sys-test");
   }

   TEST_THIS_EXPECT(bool, file_info.is_valid(), true);
   TEST_THIS_EXPECT(bool, file_info.is_file(), true);
   TEST_THIS_EXPECT(bool, file_info.is_directory(), false);
   TEST_THIS_EXPECT(bool, file_info.is_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_block_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_character_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_socket(), false);
   TEST_THIS_EXPECT(bool, file_info.is_executable(), true);

   file_info = File::get_info("/dev/sys");
   TEST_THIS_EXPECT(bool, file_info.is_valid(), true);
   TEST_THIS_EXPECT(bool, file_info.is_file(), false);
   TEST_THIS_EXPECT(bool, file_info.is_directory(), false);
   TEST_THIS_EXPECT(bool, file_info.is_device(), true);
   TEST_THIS_EXPECT(bool, file_info.is_block_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_character_device(), true);
   TEST_THIS_EXPECT(bool, file_info.is_socket(), false);
   TEST_THIS_EXPECT(bool, file_info.is_executable(), false);

   file_info = File::get_info("/dev/uart0");
   if( file_info.is_valid() ){
      TEST_THIS_EXPECT(bool, file_info.is_valid(), true);
      TEST_THIS_EXPECT(bool, file_info.is_file(), false);
      TEST_THIS_EXPECT(bool, file_info.is_directory(), false);
      TEST_THIS_EXPECT(bool, file_info.is_device(), true);
      TEST_THIS_EXPECT(bool, file_info.is_block_device(), false);
      TEST_THIS_EXPECT(bool, file_info.is_character_device(), true);
      TEST_THIS_EXPECT(bool, file_info.is_socket(), false);
      TEST_THIS_EXPECT(bool, file_info.is_executable(), false);
   }

   file_info = File::get_info("/dev");
   TEST_THIS_EXPECT(bool, file_info.is_valid(), true);
   TEST_THIS_EXPECT(bool, file_info.is_file(), false);
   TEST_THIS_EXPECT(bool, file_info.is_directory(), true);
   TEST_THIS_EXPECT(bool, file_info.is_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_block_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_character_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_socket(), false);
   TEST_THIS_EXPECT(bool, file_info.is_executable(), false);

   file_info = File::get_info("/");
   TEST_THIS_EXPECT(bool, file_info.is_valid(), true);
   TEST_THIS_EXPECT(bool, file_info.is_file(), false);
   TEST_THIS_EXPECT(bool, file_info.is_directory(), true);
   TEST_THIS_EXPECT(bool, file_info.is_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_block_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_character_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_socket(), false);
   TEST_THIS_EXPECT(bool, file_info.is_executable(), false);

   file_info = File::get_info("/app");
   TEST_THIS_EXPECT(bool, file_info.is_valid(), true);
   TEST_THIS_EXPECT(bool, file_info.is_file(), false);
   TEST_THIS_EXPECT(bool, file_info.is_directory(), true);
   TEST_THIS_EXPECT(bool, file_info.is_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_block_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_character_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_socket(), false);
   TEST_THIS_EXPECT(bool, file_info.is_executable(), false);

   file_info = File::get_info("/app/flash");
   TEST_THIS_EXPECT(bool, file_info.is_valid(), true);
   TEST_THIS_EXPECT(bool, file_info.is_file(), false);
   TEST_THIS_EXPECT(bool, file_info.is_directory(), true);
   TEST_THIS_EXPECT(bool, file_info.is_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_block_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_character_device(), false);
   TEST_THIS_EXPECT(bool, file_info.is_socket(), false);
   TEST_THIS_EXPECT(bool, file_info.is_executable(), false);

   file_info = File::get_info("/does_not_exist");
   TEST_THIS_EXPECT(bool, file_info.is_valid(), false);
   TEST_THIS_EXPECT_ERROR(-1, ENOENT);

   file_info = File::get_info("/012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");
   TEST_THIS_EXPECT(bool, file_info.is_valid(), false);
   TEST_THIS_EXPECT_ERROR(-1, ENAMETOOLONG);


   file_info = File::get_info("/01234567890/123456789012345678901234567890");
   TEST_THIS_EXPECT(bool, file_info.is_valid(), false);
   TEST_THIS_EXPECT_ERROR(-1, ENAMETOOLONG);

   return result;
}

/*@brief stress test for sys/FileInfo use "api-sys-test -file_info -stress"
 */
bool FileInfoTest::execute_class_stress_case(){
   bool result = true;
   return result;
}

/*@brief performance test for sys/FileInfo use "api-sys-test -file_info -performance"
 */
bool FileInfoTest::execute_class_performance_case(){
   bool result = true;
   return result;
}

