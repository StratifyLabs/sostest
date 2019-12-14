#include "TimerTest.hpp"
#include "sapi/chrono.hpp"
#include <sapi/sys.hpp>
TimerTest::TimerTest() : Test("chrono::Timer")
{

}
/*! \details test "api" a chrono::Timer
 *  constructors,start,resume,wait,microseconds,milliseconds,seconds
 * is_stopped,is_started,is_running,is_reset
 * @return false if some test failed
 */

bool TimerTest::execute_class_api_case(){
	bool result = true;
	const u32 delay_time_usec = 100;
	const u32 delay_time_msec = 10;
	const u32 delay_time_sec = 1;
	Timer timer_count;
	u32 time_microsecond;
	timer_count.start();
	wait(Microseconds(delay_time_usec));
	//for incredible add resume
	timer_count.resume();
	if (timer_count.microseconds() < delay_time_usec){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if (timer_count.microseconds() > 2*delay_time_usec){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}

	timer_count.stop();
	time_microsecond = timer_count.microseconds();
	if (time_microsecond < delay_time_usec){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if (time_microsecond > 2*delay_time_usec){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}

	timer_count.stop();
	if(time_microsecond != timer_count.microseconds()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	timer_count.reset();
	if(timer_count.microseconds()!=0){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	wait(Microseconds(delay_time_usec));

	if(timer_count.microseconds()!=0){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	timer_count.restart();
	wait(Microseconds(delay_time_usec));
	timer_count.stop();
	time_microsecond = timer_count.microseconds();
	if (time_microsecond < delay_time_usec){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	timer_count.stop();
	if(time_microsecond != timer_count.microseconds()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	timer_count.resume();
	wait(Microseconds(delay_time_usec));
	time_microsecond = timer_count.microseconds();
	if (time_microsecond < 2*delay_time_usec){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	timer_count.restart();
	if(timer_count.microseconds() > delay_time_usec){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(timer_count.is_stopped()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(!timer_count.is_started()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(!timer_count.is_running()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(timer_count.is_reset()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	//after stop
	timer_count.stop();
	if(!timer_count.is_stopped()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	//has been started
	if(!timer_count.is_started()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(timer_count.is_running()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(timer_count.is_reset()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	//after reset
	timer_count.reset();
	if(timer_count.microseconds()!=0){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(timer_count.milliseconds()!=0){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(timer_count.seconds()!=0){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(!timer_count.is_stopped()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	//has been started
	if(timer_count.is_started()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(timer_count.is_running()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(!timer_count.is_reset()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	//msec
	timer_count.restart();
	wait(Milliseconds(delay_time_msec));
	timer_count.stop();
	if(timer_count.milliseconds()<delay_time_msec||
			timer_count.milliseconds()>(delay_time_msec*2)){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(timer_count.milliseconds()!=(timer_count.microseconds()/1000)){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	timer_count.resume();
	wait(Milliseconds(delay_time_msec));
	if(timer_count.milliseconds()<(2*delay_time_msec)){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(timer_count.milliseconds()!=(timer_count.microseconds()/1000)){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	//sec
	timer_count.restart();
	wait(Seconds(delay_time_sec));
	timer_count.stop();
	if(timer_count.seconds() != delay_time_sec){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(timer_count.seconds()!=(timer_count.milliseconds()/1000)){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(timer_count.milliseconds()!=(timer_count.microseconds()/1000)){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	timer_count.resume();
	wait(Seconds(delay_time_sec));
	if(timer_count.seconds()<(2*delay_time_sec)){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(timer_count.seconds()!=(timer_count.milliseconds()/1000)){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if(timer_count.milliseconds()!=(timer_count.microseconds()/1000)){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	//corners value
	wait(Seconds(0));

	return result;
}
/*! \details test "stress" a chrono::Timer
 *  const and random value,
 *	mistaken
 * some time more then waiting value
 * @return false if some test failed
 */
bool TimerTest::execute_class_stress_case(){
	bool result = true;
	const u32 delay_time_usec = 500;
	u32 delay_time = 100;
	Timer timer_count;
	u32 time_usec,temp_usec;
	time_usec = 0;
	u32 max_delta_time;
	max_delta_time = 0;
	timer_count.restart();
	for (u16 i = 1;i<10000;i++){
		wait(Microseconds(delay_time_usec));
		temp_usec = timer_count.microseconds();
		if((temp_usec - (time_usec + delay_time_usec))>max_delta_time){
			max_delta_time = temp_usec - (time_usec + delay_time_usec);
		}
		if(temp_usec < (time_usec + delay_time_usec)){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
			break;
		}

		if(temp_usec > (time_usec + 2*delay_time_usec)){
			print_case_message("Warning delay time exceed double the target %d > %d", temp_usec, (time_usec + 2*delay_time_usec));
			//print_case_message("Failed %lu:%lu:%lu", temp_usec , (time_usec + 2*delay_time),i);
			break;
		}
		time_usec = timer_count.microseconds();
		//corners value
		wait(Seconds(0));

	}
	time_usec = 0;
	timer_count.restart();
	print_case_message("max delta time:%lu",max_delta_time);
	max_delta_time = 0;
	for (u16 i = 1;i<1000;i++){
		delay_time = (rand()&0xfff) + 100;
		wait(Microseconds(delay_time));
		temp_usec = timer_count.microseconds();
		if((temp_usec - (time_usec + delay_time))>max_delta_time){
			max_delta_time = temp_usec - (time_usec + delay_time);
		}
		if(temp_usec < time_usec + delay_time){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
			break;
		}
		if((temp_usec > (time_usec + 2*delay_time))/* ||
					  (temp_usec > (time_usec + delay_time + 400))*/){
			print_case_message("Warning delay time exceed double the target %d > %d", temp_usec, (time_usec + 2*delay_time_usec));
			//print_case_message("Failed %lu:%lu:%lu:%lu", delay_time,timer_count.calc_usec(), (time_usec + 2*delay_time),i);
			break;
		}
		time_usec = timer_count.microseconds();
	}
	print_case_message("max delta time:%lu",max_delta_time);
	return result;
}
