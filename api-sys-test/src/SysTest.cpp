
#include <sapi/sys.hpp>
#include <sapi/chrono.hpp>
#include <sapi/var.hpp>
#include <sapi/hal.hpp>
#include "SysTest.hpp"

SysTest::SysTest() : Test("sys::Test"){
	Data::reclaim_heap_space();
}
/*@brief api test for sys/Sys use "api-sys-test -sys -api"
 * Sys
 * launch,free_ram
 * assign_zero_sum32,verify_zero_sum32,
 * open,get_version,get_kernel_version,get_board_config,
 * get_info,get_23_info,get_26_info,get_taskattr,current_task,
 * set_current_task,get_id,
 * not writed test
 * reclaim_ram,
 * ,powerdown,hibernate,request,reset,
 * redirect_stdout,redirect_stdin,redirect_stderr,
 *
 * not tested on
 *
 */
bool SysTest::execute_class_api_case(){
	bool result;
	String version;
	String exec_destination;
	result = true;
	sos_board_config_t config;
	SysInfo sys_info;
	Sys board;

	//start fake
	TEST_THIS_EXPECT(bool,
									 Sys::launch(
										 "/app/flash/fake",
										 Sys::Arguments("orphan"),
										 Sys::DestinationPath(exec_destination),
										 Appfs::flag_is_default,
										 0,
										 nullptr,
										 Sys::Environment("")
										 ) < 0,
									 true);

	TEST_THIS_EXPECT(bool,
									 Sys::launch(
										 "/app/flash/launchslave",
										 Sys::Arguments("task_id"),
										 Sys::DestinationPath(exec_destination),
										 Appfs::flag_is_default,
										 0,
										 nullptr,
										 Sys::Environment("")
										 ) < 0,
									 false);

	wait(Microseconds(40000));

	TEST_THIS_EXPECT(int, Sys::free_ram("/app/flash/launchslave"), 0);

	version = Sys::get_kernel_version();
	TEST_THIS_EXPECT(bool, version.is_empty(), false);

	TEST_THIS_ASSERT(bool, board.open() < 0, false);
	TEST_THIS_EXPECT(int, board.get_board_config(config), 0);

	sys_info = board.get_info();
	TEST_THIS_EXPECT(bool, sys_info.is_valid(), true);

	sys_id_t sys_id;
	if(board.get_id(sys_id)!=0){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	return result;
}
/*@brief stress test for sys/Sys use "api-sys-test -sys -stress"
 *
 * not writed test
 *
 * not tested on
 */
bool SysTest::execute_class_stress_case(){
	return case_result();
}

/*@brief performsnce test for sys/Sys use "api-sys-test -sys -performance"
 *
 * not writed test
 *
 * not tested on
 */
bool SysTest::execute_class_performance_case(){
	return case_result();
}
