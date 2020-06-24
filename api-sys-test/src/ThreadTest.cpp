
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>

#include "ThreadTest.hpp"
static void * thread_4(void * args);
static int count_4 = 0;
static u32 wait_time_quatro = 4000;
int ThreadTest::count_1 = 0;
int ThreadTest::count_2 = 0;
int ThreadTest::count_3 = 0;
static bool tread_result = true;

enum Sched::policy test_get_policy(int value){
	switch (value){
		case (0):
			return Sched::OTHER;
		case(1):
			return Sched::RR;
		case(2):
			return Sched::FIFO;
		default:
			return Sched::OTHER;
	}
}
ThreadTest::ThreadTest() : Test("sys::Thread"){
	Data::reclaim_heap_space();
}
/*@brief api test for sys/Thread use "api-sys-test -thread -api"
 * get_pid,get_stacksize,set_detachstate
 * is_valid,set_priority,get_priority,get_policy
 * yield
 * not writed test
 *  join,wait
 * not tested on
 * "//check after kill"
 * @warning is_valid == true before created threads
 */
bool ThreadTest::execute_class_api_case(){
	bool result = true;
	Thread uno_thread(Thread::StackSize(4096)),
			dos_thread(Thread::StackSize(2048)),
			tres_thread(Thread::StackSize(2048)),
			quatro_thread(Thread::StackSize(2048));

	enum Sched::policy uno_policy,dos_policy,tres_policy,quatro_policy;
	int uno_priority =1;
	int dos_priority =2;
	int tres_priority =3;
	int quatro_priority =4;
	uno_policy = Sched::RR;
	dos_policy = Sched::FIFO;
	tres_policy = Sched::RR;
	quatro_policy = Sched::FIFO;
	Timer timer_count;

	TEST_THIS_EXPECT(int, uno_thread.get_detachstate(), Thread::DETACHED);
	TEST_THIS_EXPECT(bool, uno_thread.is_joinable(), false);

	TEST_THIS_EXPECT(int, uno_thread.set_detachstate(Thread::JOINABLE), 0);
	TEST_THIS_EXPECT(bool, uno_thread.is_joinable(), true);
	TEST_THIS_EXPECT(int, uno_thread.get_detachstate(), Thread::JOINABLE);

	TEST_THIS_EXPECT(int, uno_thread.set_detachstate(Thread::DETACHED), 0);
	TEST_THIS_EXPECT(bool, uno_thread.is_joinable(), false);
	TEST_THIS_EXPECT(int, uno_thread.get_detachstate(), Thread::DETACHED);

	CREATE_THREAD(uno_thread, handle_thread_1, this, uno_priority, uno_policy);
	TEST_THIS_EXPECT(bool, uno_thread.is_valid(), true);
	TEST_THIS_EXPECT(bool, uno_thread.get_pid() > 0, true);
	TEST_THIS_EXPECT(int, uno_thread.get_policy(), uno_policy);

	//repeat -- should fail before thread is busy
	CREATE_THREAD_ERROR(EBUSY, uno_thread, handle_thread_1, this, uno_priority+1, uno_policy);

	TEST_THIS_EXPECT(bool, dos_thread.is_valid(), false);

	TEST_THIS_EXPECT(int, dos_thread.set_detachstate(Thread::DETACHED), 0);
	TEST_THIS_EXPECT(int, dos_thread.get_detachstate(), Thread::DETACHED);
	TEST_THIS_EXPECT(bool, dos_thread.is_joinable(), false);
	TEST_THIS_EXPECT(int, dos_thread.set_detachstate(Thread::JOINABLE), 0);
	TEST_THIS_EXPECT(int, dos_thread.get_detachstate(), Thread::JOINABLE);
	TEST_THIS_EXPECT(bool, dos_thread.is_joinable(), true);

	timer_count.start();


	CREATE_THREAD(dos_thread,handle_thread_2,this,dos_priority,dos_policy);
	TEST_THIS_EXPECT(bool, dos_thread.is_valid(), true);

	TEST_THIS_EXPECT(bool, dos_thread.get_pid() > 0, true);

	TEST_THIS_EXPECT(int, dos_thread.get_policy(), dos_policy);
	CREATE_THREAD(tres_thread, handle_thread_3, this, tres_priority, tres_policy);

	TEST_THIS_EXPECT(bool, dos_thread.is_valid(), true);
	TEST_THIS_EXPECT(bool, tres_thread.get_pid() > 0, true);
	TEST_THIS_EXPECT(int, tres_thread.get_policy(), tres_policy);

	CREATE_THREAD(quatro_thread, thread_4, this, quatro_priority, quatro_policy);

	TEST_THIS_EXPECT(bool, dos_thread.is_valid(), true);
	TEST_THIS_EXPECT(bool, quatro_thread.get_pid() > 0, true);
	TEST_THIS_EXPECT(int, quatro_thread.get_policy(), quatro_policy);

	TEST_THIS_EXPECT(int, uno_thread.get_priority(), uno_priority);
	TEST_THIS_EXPECT(int, dos_thread.get_priority(), dos_priority);
	TEST_THIS_EXPECT(int, tres_thread.get_priority(), tres_priority);
	TEST_THIS_EXPECT(int, quatro_thread.get_priority(), quatro_priority);

	print_case_message("Priority test");
	for(int i=0;i< Sched::get_priority_max(uno_policy);i++){
		uno_priority= (uno_priority>=Sched::get_priority_max(uno_policy))?uno_priority:uno_priority+1;
		dos_priority= (dos_priority>=Sched::get_priority_max(dos_policy))?dos_priority:dos_priority+1;
		tres_priority= (tres_priority>=Sched::get_priority_max(tres_policy))?Sched::get_priority_max(tres_policy):tres_priority+1;
		quatro_priority= (quatro_priority>=Sched::get_priority_max(quatro_policy))?Sched::get_priority_max(quatro_policy):quatro_priority+1;
		TEST_THIS_EXPECT(int, uno_thread.set_priority(uno_priority), 0);
		TEST_THIS_EXPECT(int, dos_thread.set_priority(dos_priority), 0);
		TEST_THIS_EXPECT(int, tres_thread.set_priority(tres_priority), 0);
		TEST_THIS_EXPECT(int, quatro_thread.set_priority(quatro_priority), 0);

		TEST_THIS_EXPECT(int, uno_thread.get_priority(), uno_priority);
		TEST_THIS_EXPECT(int, dos_thread.get_priority(), dos_priority);
		TEST_THIS_EXPECT(int, tres_thread.get_priority(), tres_priority);
		TEST_THIS_EXPECT(int, quatro_thread.get_priority(), quatro_priority);
	}

	TEST_THIS_EXPECT(int, uno_priority, Sched::get_priority_max(uno_policy));

	print_case_message("Yield to threads");
	if(uno_thread.is_running()||
		 dos_thread.is_running()||
		 tres_thread.is_running()||
		 quatro_thread.is_running()){
		Thread::yield();//self proccess
	}

	//stack size
	TEST_THIS_EXPECT(bool, uno_thread.get_stacksize() > 0, true);

	TEST_THIS_EXPECT(int, count_1, 0);
	TEST_THIS_EXPECT(int, count_2, 0);
	TEST_THIS_EXPECT(int, count_3, 0);
	TEST_THIS_EXPECT(int, count_4, 0);

	//    void** temp;
	//    pid_t current_pid;

	//    current_pid = Sched::get_pid();
	print_case_message("Join uno");
	TEST_THIS_EXPECT(int, uno_thread.join(), 0);
	print_case_message("Join dos");
	TEST_THIS_EXPECT(int, dos_thread.join(), 0);

	//    uno_thread.wait(temp,1500);
	//    print_case_message("timer after join %lu",timer_count.microseconds());
	//while before all thread is stoped
	while(uno_thread.is_running()||
				dos_thread.is_running()||
				tres_thread.is_running()||
				quatro_thread.is_running()){
		Thread::yield();
	}
	//    print_case_message("timer after join %lu",timer_count.microseconds());

	TEST_THIS_EXPECT(bool, count_1 < 2, false);
	TEST_THIS_EXPECT(bool, count_2 < 2, false);
	TEST_THIS_EXPECT(bool, count_3 < 2, false);
	TEST_THIS_EXPECT(bool, count_4 < 2, false);

	TEST_THIS_EXPECT(bool, timer_count.microseconds()< wait_time, false);

	print_case_message("Repeat create uno");

	//repeat create thread
	TEST_THIS_EXPECT(int, uno_thread.set_detachstate(Thread::DETACHED), 0);
	CREATE_THREAD(uno_thread,handle_thread_1,this,uno_priority,uno_policy);


	if(uno_thread.is_running()){
#if 0 //Signal::CONT causes the OS to stall Issue
		if( uno_thread.kill(Signal::STOP) < 0 ){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
		}

		if( uno_thread.kill(Signal::CONT) < 0 ){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
		}
#endif

	} else {
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}

	uno_thread.wait(
				Thread::Return(nullptr),
				Thread::DelayInterval(Microseconds(100))
				);

	//check after kill

	TEST_THIS_EXPECT(bool, uno_thread.is_running(), false);

	return result;

}
/* @brief performance test for sys/Thread use "api-sys-test -thread -performance"
 * not writed: test add join
 *   thread start with priority bring check
 * not tested on
 * @warning create with OTHER parametr(test_get_policy return OTHER policy)
 */
bool ThreadTest::execute_class_performance_case(){
	bool result = true;
	const u32 itterate_num = 16;
	Thread uno_thread(Thread::StackSize(4096)),
			dos_thread(Thread::StackSize(2048)),
			tres_thread(Thread::StackSize(2048)),
			quatro_thread(Thread::StackSize(2048));

	//pid_t uno_id,dos_id,tres_id,quatro_id;
	enum Sched::policy policy;
	u8 priority=4;
	u8 max_priority;
	u8 min_priority;
	//RR sheduler
	Timer timer_count;
	for(u32 i=0;i<itterate_num;i++){
		u32 max_time;
		timer_count.start();
		count_1=0;count_2=0;count_3=0;count_4=0;
		max_time = wait_time>wait_time_quatro?wait_time:wait_time_quatro;//use max value
		policy  = test_get_policy(i%3);

		min_priority = Sched::get_priority_min(policy);
		max_priority= Sched::get_priority_max(policy);
		priority= (i % max_priority);
		priority = priority < min_priority ? min_priority : priority;
		priority = priority > max_priority ? max_priority : priority;


		CREATE_THREAD(uno_thread,handle_thread_1,this,priority,policy);
		CREATE_THREAD(dos_thread,handle_thread_2,this,priority,policy);
		CREATE_THREAD(tres_thread,handle_thread_3,this,priority,policy);
		CREATE_THREAD(quatro_thread,thread_4,nullptr,priority,policy);

		while( uno_thread.is_running()||dos_thread.is_running()||
					 tres_thread.is_running()||quatro_thread.is_running() ){
			Thread::yield();
		}
		if((count_1 != 2)||(count_2!=2)||(count_3!=2)||(count_4!=2)){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
			result = false;
			break;
		}
		//wait_time sets only in joined thread
		if(timer_count.microseconds()< max_time){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
			result = false;
			break;
		}
		uno_thread.wait(Thread::Return(nullptr), Thread::DelayInterval(Microseconds(500)));
		dos_thread.wait(Thread::Return(nullptr), Thread::DelayInterval(Microseconds(500)));
		tres_thread.wait(Thread::Return(nullptr), Thread::DelayInterval(Microseconds(500)));
		quatro_thread.wait(Thread::Return(nullptr), Thread::DelayInterval(Microseconds(500)));
		timer_count.stop();
	}
	return result;
}
/* @brief stress test for sys/Thread use "api-sys-test -thread -stress"
 * not writed: test add join
 *   thread start with priority bring check
 * not tested on
 * @warning create with OTHER parametr
 */
bool ThreadTest::execute_class_stress_case(){
	bool result = true;
	const int itterate_num = 512;
	Thread uno_thread(Thread::StackSize(4096)),
			dos_thread(Thread::StackSize(2048)),
			tres_thread(Thread::StackSize(2048)),
			quatro_thread(Thread::StackSize(2048));
	//pid_t uno_id,dos_id,tres_id,quatro_id;
	enum Sched::policy uno_policy,dos_policy,tres_policy,quatro_policy;
	int uno_priority=4;
	int dos_priority=4;
	int tres_priority =4;
	int quatro_priority =4;
	int max_prior_RR, max_prior_FIFO;
	int min_prior_RR, min_prior_FIFO;

	//RR sheduler
	uno_policy = Sched::RR;
	dos_policy = Sched::RR;
	tres_policy = Sched::RR;
	quatro_policy = Sched::RR;
	Timer timer_count;
	max_prior_RR= Sched::get_priority_max(Sched::RR);
	max_prior_RR= 20;
	max_prior_FIFO= Sched::get_priority_max(Sched::FIFO);
	max_prior_FIFO= 20;
	min_prior_RR = Sched::get_priority_min(Sched::RR);
	min_prior_FIFO = Sched::get_priority_min(Sched::FIFO);

	print_case_message("vary policy and priority");

	for(int i=0;i<itterate_num;i++){
		u32 max_time;
		wait_time_quatro = ((u16)rand() & 0x1ff) + 5000;//used for quatro thread
		wait_time = ((u16)rand() & 0x1ff)+ 5000;//used for thread uno dos tres
		max_time = wait_time>wait_time_quatro?wait_time:wait_time_quatro;//use max value
		timer_count.start();
		count_1=0;count_2=0;count_3=0;count_4=0;
		uno_priority= rand() % max_prior_RR;
		uno_priority= uno_priority<min_prior_RR?min_prior_RR:uno_priority;
		dos_priority= rand() % max_prior_RR;
		dos_priority= dos_priority<min_prior_RR?min_prior_RR:dos_priority;
		tres_priority= rand() % max_prior_RR;
		tres_priority= tres_priority<min_prior_RR?min_prior_RR:tres_priority;
		quatro_priority= rand() % max_prior_RR;
		quatro_priority= quatro_priority<min_prior_RR?min_prior_RR:quatro_priority;

		CREATE_THREAD(uno_thread,handle_thread_1,this,uno_priority,uno_policy);
		CREATE_THREAD(dos_thread,handle_thread_2,this,dos_priority,dos_policy);
		CREATE_THREAD(tres_thread,handle_thread_3,this,tres_priority,tres_policy);
		CREATE_THREAD(quatro_thread,thread_4,nullptr,quatro_priority,quatro_policy);

		uno_priority= ((i % max_prior_RR)<min_prior_RR)?min_prior_RR:(i % max_prior_RR);

		TEST_THIS_EXPECT(bool, uno_thread.set_priority(uno_priority) < 0, false);
		if( uno_thread.set_priority(uno_priority) < 0 ){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
			result = false;
		}

		TEST_THIS_EXPECT(int, uno_thread.get_priority(), uno_priority);
		if (uno_thread.get_priority()!= uno_priority){
			print_case_message("Failed in cycle %s:%d:%d (%d!=%d)", __PRETTY_FUNCTION__, __LINE__, i, uno_thread.get_priority(), uno_priority);
			result = false;
		}

		//priority isn't valid until after thread has been created
		dos_priority= ((i % max_prior_RR)<min_prior_RR)?min_prior_RR:(i % max_prior_RR);
		TEST_THIS_EXPECT(bool, dos_thread.set_priority(dos_priority) < 0, false);
		TEST_THIS_EXPECT(int, dos_thread.get_priority(), dos_priority);


		while(
					uno_thread.is_running()||
					dos_thread.is_running()||
					tres_thread.is_running()||
					quatro_thread.is_running()
					){
			Thread::yield();
		}

		TEST_THIS_EXPECT(bool, dos_thread.set_priority(dos_priority) < 0, false);
		if((count_1 != 2)||(count_2!=2)||(count_3!=2)||(count_4!=2)){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
			result = false;
		}
		//wait_time sets only in joined thread
		if(timer_count.microseconds()< max_time){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
			result = false;
		}

		uno_thread.wait(Thread::Return(nullptr), Thread::DelayInterval(Microseconds(500)));
		dos_thread.wait(Thread::Return(nullptr), Thread::DelayInterval(Microseconds(500)));
		tres_thread.wait(Thread::Return(nullptr), Thread::DelayInterval(Microseconds(500)));
		quatro_thread.wait(Thread::Return(nullptr), Thread::DelayInterval(Microseconds(500)));
		timer_count.stop();
		if( !result ){
			//break;
		}
	}
	//FIFO sheduler
	uno_policy = Sched::FIFO;
	dos_policy = Sched::FIFO;
	tres_policy = Sched::FIFO;
	quatro_policy = Sched::FIFO;
	print_case_message("test fifo policy");
	for(int i=0;i<itterate_num;i++){
		u32 max_time;
		uno_priority= rand() % max_prior_FIFO;
		uno_priority= uno_priority<max_prior_FIFO?max_prior_FIFO:uno_priority;
		dos_priority= rand() % max_prior_FIFO;
		dos_priority= dos_priority<max_prior_FIFO?max_prior_FIFO:dos_priority;
		tres_priority= rand() % max_prior_FIFO;
		tres_priority= tres_priority<max_prior_FIFO?max_prior_FIFO:tres_priority;
		quatro_priority= rand() % max_prior_FIFO;
		quatro_priority= quatro_priority<max_prior_FIFO?max_prior_FIFO:quatro_priority;
		wait_time_quatro = ((u16)rand() & 0x1ff) +5000;//used for quatro thread
		wait_time = ((u16)rand() & 0x1ff)+5000;//used for thread uno dos tres
		max_time = wait_time>wait_time_quatro?wait_time:wait_time_quatro;//use max value
		timer_count.start();
		count_1=0;count_2=0;count_3=0;count_4=0;

		CREATE_THREAD(uno_thread,handle_thread_1,this,uno_priority,uno_policy);
		CREATE_THREAD(dos_thread,handle_thread_2,this,dos_priority,dos_policy);
		CREATE_THREAD(tres_thread,handle_thread_3,this,tres_priority,tres_policy);
		CREATE_THREAD(quatro_thread,thread_4,nullptr,quatro_priority,quatro_policy);

		uno_priority= ((i % max_prior_FIFO)<min_prior_FIFO)?min_prior_FIFO:(i % max_prior_FIFO);
		uno_thread.set_priority(uno_priority);
		TEST_THIS_EXPECT(int, uno_thread.get_priority(), uno_priority);

		dos_priority= ((i % max_prior_FIFO)<min_prior_FIFO)?min_prior_FIFO:(i % max_prior_FIFO);
		dos_thread.set_priority(dos_priority);
		TEST_THIS_EXPECT(int, dos_thread.get_priority(), dos_priority);


		while(uno_thread.is_running()||
					dos_thread.is_running()||
					tres_thread.is_running()||
					quatro_thread.is_running()){
			Thread::yield();
		}
		if((count_1 != 2)||(count_2!=2)||(count_3!=2)||(count_4!=2)){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
			result = false;
			break;
		}
		//wait_time sets only in joined thread
		if(timer_count.microseconds()< max_time){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
			result = false;
			break;
		}

		uno_thread.kill(0);
		dos_thread.kill(0);
		tres_thread.kill(0);
		quatro_thread.kill(0);
		timer_count.stop();
	}
	//FIFO and RR sheduler
	uno_policy = Sched::FIFO;
	dos_policy = Sched::RR;
	tres_policy = Sched::FIFO;
	quatro_policy = Sched::RR;
	print_case_message("test fifo/RR policy");
	for(int i=0;i<itterate_num;i++){
		u32 max_time;
		uno_priority= rand() % max_prior_FIFO;
		uno_priority= uno_priority<max_prior_FIFO?max_prior_FIFO:uno_priority;
		dos_priority= rand() % max_prior_RR;
		dos_priority= dos_priority<max_prior_RR?max_prior_RR:dos_priority;
		tres_priority= rand() % max_prior_FIFO;
		tres_priority= tres_priority<max_prior_FIFO?max_prior_FIFO:tres_priority;
		quatro_priority= rand() % max_prior_RR;
		quatro_priority= quatro_priority<max_prior_RR?max_prior_RR:quatro_priority;

		wait_time_quatro = ((u16)rand() & 0x1ff) +5000;//used for quatro thread
		wait_time = ((u16)rand() & 0x1ff)+5000;//used for thread uno dos tres
		max_time = wait_time>wait_time_quatro?wait_time:wait_time_quatro;//use max value
		timer_count.start();
		count_1=0;count_2=0;count_3=0;count_4=0;

		CREATE_THREAD(uno_thread,handle_thread_1,this,uno_priority,uno_policy);
		CREATE_THREAD(dos_thread,handle_thread_2,this,dos_priority,dos_policy);
		CREATE_THREAD(tres_thread,handle_thread_3,this,tres_priority,tres_policy);
		CREATE_THREAD(quatro_thread,thread_4,nullptr,quatro_priority,quatro_policy);

		uno_priority= ((i % max_prior_FIFO)<min_prior_FIFO)?min_prior_FIFO:(i % max_prior_FIFO);
		uno_thread.set_priority(uno_priority);
		TEST_THIS_EXPECT(int, uno_thread.get_priority(), uno_priority);

		dos_priority= ((i % max_prior_RR)<min_prior_RR)?min_prior_RR:(i % max_prior_RR);
		dos_thread.set_priority(dos_priority);
		TEST_THIS_EXPECT(int, dos_thread.get_priority(), dos_priority);

		while(uno_thread.is_running()||
					dos_thread.is_running()||
					tres_thread.is_running()||
					quatro_thread.is_running()){
			Thread::yield();
		}
		if((count_1 != 2)||(count_2!=2)||(count_3!=2)||(count_4!=2)){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
			result = false;
			break;
		}
		//wait_time sets only in joined thread
		if(timer_count.microseconds()< max_time){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
			result = false;
			break;
		}

		uno_thread.kill(0);
		dos_thread.kill(0);
		tres_thread.kill(0);
		quatro_thread.kill(0);
		timer_count.stop();
	}
	//FIFO and RR sheduler
	uno_policy = Sched::OTHER;
	dos_policy = Sched::OTHER;
	tres_policy = Sched::OTHER;
	quatro_policy = Sched::OTHER;
	print_case_message("test other policy");
	for(int i=0;i<itterate_num;i++){
		u32 max_time;
		uno_priority = 0;
		dos_priority = 0;
		tres_priority = 0;
		quatro_priority = 0;
		wait_time_quatro = ((u16)rand() & 0x1ff) +5000;//used for quatro thread
		wait_time = ((u16)rand() & 0x1ff)+5000;//used for thread uno dos tres
		max_time = wait_time>wait_time_quatro?wait_time:wait_time_quatro;//use max value
		timer_count.start();
		count_1=0;count_2=0;count_3=0;count_4=0;

		CREATE_THREAD(uno_thread,handle_thread_1,this,uno_priority,uno_policy);
		CREATE_THREAD(dos_thread,handle_thread_2,this,dos_priority,dos_policy);
		CREATE_THREAD(tres_thread,handle_thread_3,this,tres_priority,tres_policy);
		CREATE_THREAD(quatro_thread,thread_4,nullptr,quatro_priority,quatro_policy);


		uno_thread.set_priority(uno_priority);
		TEST_THIS_EXPECT(int, uno_thread.get_priority(), uno_priority);

		dos_thread.set_priority(dos_priority);
		TEST_THIS_EXPECT(int, dos_thread.get_priority(), dos_priority);
		if (dos_thread.get_priority()!= dos_priority){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
			result = false;
			break;
		}

		while(uno_thread.is_running()||
					dos_thread.is_running()||
					tres_thread.is_running()||
					quatro_thread.is_running()){
			Thread::yield();
		}

		TEST_THIS_EXPECT(int, count_1, 2);
		TEST_THIS_EXPECT(int, count_2, 2);
		TEST_THIS_EXPECT(int, count_3, 2);
		TEST_THIS_EXPECT(int, count_4, 2);

		//wait_time sets only in joined thread
		if(timer_count.microseconds() < max_time){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
			result = false;
			break;
		}

		uno_thread.kill(0);
		dos_thread.kill(0);
		tres_thread.kill(0);
		quatro_thread.kill(0);
		timer_count.stop();
	}

	TEST_THIS_EXPECT(bool, tread_result, true);
	return result;
}

void * ThreadTest::thread_1(u32 wait_time){
	Queue <u8>u8_queue;
	Queue <u32>u32_queue;
	Queue <double>double_queue;
	count_1++;
	wait(Microseconds(wait_time));
	count_1++;
	//add test from queue api_var_test(make it more difficult)
	for (u32 i = 0;i<500;i++){
		u8_queue.push(i);
		u32_queue.push(i);
		double_queue.push((double)i);
	}
	for (u32 i = 0;i<500;i++){
		if((u8_queue.front()!=(u8)i)||(u32_queue.front()!=i)||(double_queue.front()!=(double)i)){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			tread_result = false;
			break;
		}
		u8_queue.pop();
		u32_queue.pop();
		double_queue.pop();
	}
	if(!u8_queue.is_empty() || !u32_queue.is_empty() || !double_queue.is_empty()){
		u8_queue.clear();
		u32_queue.clear();
		double_queue.clear();
		tread_result = false;
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
	}
	return &count_1;
}
void * ThreadTest::thread_2(u32 wait_time){
	count_2++;
	m_thread_2_timer.restart();
	wait(Microseconds(wait_time));
	m_thread_2_timer.stop();
	count_2++;
	return &count_2;
}
void * ThreadTest::thread_3(u32 wait_time){
	count_3++;
	wait(Microseconds(wait_time));
	count_3++;
	return &count_3;
}

static void * thread_4(void * args){
	count_4++;
	wait(Microseconds(wait_time_quatro));
	count_4++;
	return &count_4;
}
