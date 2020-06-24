#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include <sapi/fs.hpp>
#include <sapi/fmt.hpp>
#include "AppfsTest.hpp"

AppfsTest::AppfsTest() : Test("sys::Appfs"){

}

/*@brief api test for sys/appfs use "api-sys-test -appfs -api"
 * @warning
 * Appfs::create != 0
 */
bool AppfsTest::execute_class_api_case(){
	String file_name = "my_data_1";
	AppfsInfo info;

	String my_user_data;
	my_user_data << "Hello World 1";
	Data my_user_data_read_back(my_user_data.length());

	DataFile user_data_file;
	//user_data_file.data().refer_to(my_user_data);

	TEST_THIS_EXPECT(
				bool, Appfs::create(
					Appfs::Name(file_name),
					user_data_file
					) < 0,
				false);


	//read back the file that was created
	File f;
	String path = String("/app/flash/") + file_name;
	TEST_THIS_ASSERT(bool, f.open(
										 path,
										 OpenFlags::read_only()
										 ) < 0,
									 false);

#if 1
	TEST_THIS_EXPECT(bool,
									 f.read(my_user_data_read_back) == (int)my_user_data_read_back.size(),
									 true);
#endif

	TEST_THIS_EXPECT(int,
									 f.close(),
									 0);

	TEST_THIS_EXPECT(String, String(my_user_data_read_back), my_user_data);

	info = Appfs::get_info(path).info();
	TEST_THIS_EXPECT(bool, info.is_valid(), true);
	TEST_THIS_EXPECT(String, info.name(), file_name);
	return case_result();
}

/*@brief stress test for sys/appfs use "api-sys-test -appfs -stress"
 */
bool AppfsTest::execute_class_stress_case(){
	bool result = true;
	return result;
}

/*@brief performance test for sys/appfs use "api-sys-test -appfs -performance"
 */
bool AppfsTest::execute_class_performance_case(){
	bool result = true;
	return result;
}
