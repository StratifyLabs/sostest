#include "TimeTest.hpp"
#include "sapi/chrono.hpp"
#include "ctime"
#include <sapi/hal.hpp>
TimeTest::TimeTest() : Test("chrono::Time"){

}
static bool time_compare(tm time_tm_uno,tm time_tm_dos);
/*! \details test "api" a chrono::Time
 *  constructors,
 * @return false if some test failed
 */
bool TimeTest::execute_class_api_case(){
	bool result = true;
	MicroTimer timer_sh;
	Time now;
	time_t current_time_uno,current_time_dos;
	tm time_tm_uno,time_tm_dos;
	u8 const time_delay_sec =1;
	Device rtc;
	rtc.open("/dev/rtc", O_RDWR);
	rtc_attr_t attr;
	attr.o_flags = RTC_FLAG_ENABLE;
	rtc.ioctl(I_RTC_SETATTR, &attr);
	timer_sh.start();
	current_time_uno = now.get_time_of_day();
	time_tm_uno = now.get_tm();
	timer_sh.wait_sec(time_delay_sec);
	current_time_dos = now.get_time_of_day();
	time_tm_dos = now.get_tm();
	if(time_compare(time_tm_uno,time_tm_dos)){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(current_time_dos <= current_time_uno){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}

	now.set_value(1526990058);
	struct tm time_struct;
	time_t m_time = 1526990058;
	gmtime_r(&m_time, &time_struct);
	now.set_time_of_day();
	current_time_dos = now.get_time_of_day();
	time_tm_dos = now.get_tm();
	if(time_tm_dos.tm_year != time_struct.tm_year){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(time_tm_dos.tm_mon != time_struct.tm_mon ){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(time_tm_dos.tm_hour != time_struct.tm_hour){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	timer_sh.wait_sec(time_delay_sec);
	current_time_dos = now.get_time_of_day();
	time_tm_dos = now.get_tm();
	if(time_tm_dos.tm_year != time_struct.tm_year){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(time_tm_dos.tm_mon != time_struct.tm_mon ){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(time_tm_dos.tm_hour != time_struct.tm_hour ){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(time_tm_dos.tm_sec == time_struct.tm_sec){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}

	rtc.close();
	return result;
}

bool TimeTest::execute_class_stress_case(){
	bool result = true;

	return result;
}
static bool time_compare(tm time_tm_uno,tm time_tm_dos){
	bool result;
	result =0;
	if(time_tm_uno.tm_hour==time_tm_dos.tm_hour&&
			time_tm_uno.tm_min== time_tm_dos.tm_min&&
			time_tm_uno.tm_sec== time_tm_dos.tm_sec&&
			time_tm_uno.tm_year==time_tm_dos.tm_year&&
			time_tm_uno.tm_yday==time_tm_dos.tm_yday&&
			time_tm_uno.tm_wday==time_tm_dos.tm_wday&&
			time_tm_uno.tm_mon== time_tm_dos.tm_mon){
		result =1;
	}
	return result;
}
