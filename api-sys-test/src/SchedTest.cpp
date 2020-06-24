#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <errno.h>

#include "SchedTest.hpp"
SchedTest::SchedTest():Test("sys::Sched"){
	Data::reclaim_heap_space();
}
/*@brief test api for sched
 * RR and FIFO policy
 * get_priority_max,get_priority_min,
 * get_pid,set_scheduler,get_priority
 * get_rr_interval
 * */
bool SchedTest::execute_class_api_case(){
	pid_t process_id;
	int rr_priority_max,rr_priority_min;
	int fifo_priority_max,fifo_priority_min;
	int other_priority_max,other_priority_min;
	const int priority_rr = 5;
	const int priority_fifo = 4;
	//    const int priority_other = 3;
	int priority;
	int rr_interval;

	rr_priority_max = Sched::get_priority_max(Sched::RR);
	TEST_THIS_EXPECT(bool, rr_priority_max > 0, true);
	rr_priority_min = Sched::get_priority_min(Sched::RR);
	TEST_THIS_EXPECT(bool, rr_priority_min<0, false);
	fifo_priority_max = Sched::get_priority_max(Sched::FIFO);
	TEST_THIS_EXPECT(bool, fifo_priority_max<=0, false);
	fifo_priority_min = Sched::get_priority_min(Sched::FIFO);
	TEST_THIS_EXPECT(bool, fifo_priority_min<0, false);
	other_priority_max = Sched::get_priority_max(Sched::OTHER);
	TEST_THIS_EXPECT(bool, other_priority_max<0, false);
	other_priority_min = Sched::get_priority_min(Sched::OTHER);
	TEST_THIS_EXPECT(bool, other_priority_min<0, false);

	for(u32 i = 10000; i < 50000; i++){
		SET_SCHEDULER_ERROR(ESRCH, i, Sched::RR, 0);
		SET_SCHEDULER_ERROR(ESRCH, i, Sched::FIFO, 0);
		SET_SCHEDULER_ERROR(ESRCH, i, Sched::OTHER, 0);
	}

	//get proccess id
	process_id = Sched::get_pid();
	TEST_THIS_EXPECT(bool, process_id<=0, false);
	Sched::yield();
	SET_SCHEDULER(process_id, Sched::RR, priority_rr);
	Sched::yield();
	priority = Sched::get_priority(Sched::ProcessId(process_id));
	TEST_THIS_EXPECT(int, priority, priority_rr);
	rr_interval = Sched::get_rr_interval(Sched::ProcessId(process_id));
	TEST_THIS_EXPECT(bool, rr_interval > 0, true);
	SET_SCHEDULER(process_id, Sched::FIFO, priority_fifo);
	Sched::yield();
	priority = Sched::get_priority(Sched::ProcessId(process_id));
	TEST_THIS_EXPECT(int, priority, priority_fifo);
	rr_interval = Sched::get_rr_interval(Sched::ProcessId(process_id));
	TEST_THIS_EXPECT(bool, rr_interval > 0, true);
	SET_SCHEDULER_ERROR(EINVAL, process_id, Sched::FIFO, fifo_priority_max+1);
	Sched::yield();
	SET_SCHEDULER_ERROR(EINVAL, process_id, Sched::RR, rr_priority_max+1);
	Sched::yield();
	SET_SCHEDULER_ERROR(EINVAL, process_id, Sched::FIFO, fifo_priority_min-1);
	Sched::yield();
	SET_SCHEDULER_ERROR(EINVAL, process_id, Sched::RR, rr_priority_min-1);

	for(u32 i=1; i < 50000; i++){
		SET_SCHEDULER_ERROR(EINVAL, process_id, Sched::OTHER, i);
	}

	return case_result();
}
/* @brief stress test for sys Sched
 *
 * not tested on
 * @warning set sheduller with OTHER param
 */
bool SchedTest::execute_class_stress_case(){
	bool result = true;
	pid_t process_id;
	int priority_set;
	int priority_get;
	int fifo_priority_min,fifo_priority_max;
	int rr_priority_min,rr_priority_max;
	int other_priority_min,other_priority_max;
	int rr_interval;
	rr_priority_max = Sched::get_priority_max(Sched::RR);
	fifo_priority_max = Sched::get_priority_max(Sched::FIFO);
	rr_priority_min = Sched::get_priority_min(Sched::RR);
	fifo_priority_min = Sched::get_priority_min(Sched::FIFO);
	other_priority_min = Sched::get_priority_min(Sched::OTHER);
	other_priority_max = Sched::get_priority_max(Sched::OTHER);
	const u16 itteration_number = 1000;



	for(u16 i=0; i < itteration_number; i++){
		//get proccess id
		process_id = Sched::get_pid();
		priority_set = i % fifo_priority_max;
		priority_set = priority_set < fifo_priority_min?fifo_priority_min:priority_set;
		if(process_id <= 0){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
			break;
		}


		SET_SCHEDULER(process_id, Sched::FIFO, priority_set);

		Sched::yield();
		priority_get = Sched::get_priority(Sched::ProcessId(process_id));
		TEST_THIS_EXPECT(int, priority_get, priority_set);
		rr_interval = Sched::get_rr_interval(Sched::ProcessId(process_id));
		TEST_THIS_EXPECT(bool, rr_interval <= 0, false);

		priority_set = i % rr_priority_max;
		priority_set = priority_set < rr_priority_min?rr_priority_min:priority_set;
		TEST_THIS_EXPECT(bool, process_id <= 0, false);

		SET_SCHEDULER(process_id, Sched::RR, priority_set);
		Sched::yield();
		priority_get = Sched::get_priority(Sched::ProcessId(process_id));
		TEST_THIS_EXPECT(int, priority_get, priority_set);

		rr_interval = Sched::get_rr_interval(Sched::ProcessId(process_id));
		TEST_THIS_EXPECT(bool, rr_interval <= 0, false);

		priority_set = i % other_priority_max;
		priority_set = priority_set < other_priority_min?other_priority_min:priority_set;
		TEST_THIS_EXPECT(bool, process_id <= 0, false);

		SET_SCHEDULER(process_id, Sched::OTHER, 0);

		Sched::yield();
		priority_get = Sched::get_priority(Sched::ProcessId(process_id));
		TEST_THIS_EXPECT(int, priority_get, 0);

		rr_interval = Sched::get_rr_interval(Sched::ProcessId(process_id));
		TEST_THIS_EXPECT(bool, rr_interval <= 0, false);
	}
	return result;
}
/* @brief performance test for sys Sched
 * not tested on
 * @warning set sheduller with OTHER param
 */
bool SchedTest::execute_class_performance_case(){

	//this should measure the performance of the scheduler (how long to context switches take)

	return true;
}
