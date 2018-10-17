#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include <sapi/fmt.hpp>
#include "AppfsTest.hpp"
AppfsTest::AppfsTest() : Test("sys::Appfs"){

}
static bool make_file_in_flash(const ConstString &	file_name);
static void rand_string_value(u16 size,String & string);
static u8 pseudo_rnd_u8() ;
static const u16 page_size = 32;

/*@brief api test for sys/appfs use "api-sys-test -appfs -api"
 * @warning
 * Appfs::create != 0
 */
bool AppfsTest::execute_class_api_case(){
	int result;
	ConstString file_name = "my_data_1";
	appfs_info_t info;

	String my_user_data;
	my_user_data << "Hello World 1";
	String my_user_data_read_back;

	my_user_data_read_back.set_capacity(my_user_data.size());


	if( (result = Appfs::create(file_name, my_user_data)) < 0 ){
		print_case_failed("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		return case_result();
	}


	//read back the file that was created
	File f;
	String path("/app/flash/");
	path << file_name;

	if( f.open(path, File::RDONLY) < 0 ){
		print_case_failed("Failed to read appfs file at %s", path.to_char());
	} else {
		print_case_message("Read %d bytes", f.read(my_user_data_read_back)); //read 16 bytes of user data
		if( my_user_data != my_user_data_read_back ){
			print_case_failed("Failed %s:%d (%s, %s)", __PRETTY_FUNCTION__, __LINE__, my_user_data.str(), my_user_data_read_back.str());
		}
		f.close();  //free resources
	}

	Appfs::get_info(path,info);
	if( errno != ENOEXEC ){
		print_case_failed("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
	}

	Appfs::get_info(file_name,info);
	if( errno != ENOENT ){
		print_case_failed("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
	}

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
