#include <sapi/sys.hpp>
#include "ThreadTest.hpp"
static void * thread_4(void * args);
static int count_4 = 0;
static u32 wait_time_quatro = 4000;
int ThreadTest::count_1 = 0;
int ThreadTest::count_2 = 0;
int ThreadTest::count_3 = 0;


ThreadTest::ThreadTest() : Test("sys::Thread"){
}
/*@brief api test for sys/Thread use "api-sys-test -thread -api"
 * get_pid,get_stacksize,set_detachstate
 * is_valid,set_priority,get_priority,get_policy
 * yield
 * not writed test
 *  join,wait
 *
 * not tested on
 * "//check after kill"
 */
bool ThreadTest::execute_class_api_case(){
    bool result = true;
    Thread uno_thread,dos_thread,tres_thread,quatro_thread;
    pid_t uno_id,dos_id,tres_id,quatro_id;
    enum Sched::policy uno_policy,dos_policy,tres_policy,quatro_policy;
    int uno_priority =1;
    int dos_priority =2;
    int tres_priority =3;
    int quatro_priority =4;
    uno_policy = Sched::RR;
    dos_policy = Sched::FIFO;
    tres_policy = Sched::RR;
    quatro_policy = Sched::FIFO;

    if(uno_thread.get_detachstate()!=PTHREAD_CREATE_DETACHED){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(uno_thread.is_joinable()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(uno_thread.create(handle_thread_1,this,uno_priority,uno_policy)==-1){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!uno_thread.is_valid()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    uno_id = uno_thread.get_pid();
    if(!uno_id){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(uno_thread.get_policy() != uno_policy){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //repeat
    if(!(uno_thread.create(handle_thread_1,this,uno_priority+1,uno_policy)==-1)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(dos_thread.is_valid()){//check before create
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(dos_thread.create(handle_thread_2,this,dos_priority,dos_policy)==-1){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    dos_id = dos_thread.get_pid();
    if(!dos_id){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(dos_thread.get_policy() != dos_policy){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(tres_thread.create(handle_thread_3,this,tres_priority,tres_policy)==-1){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!tres_thread.is_valid()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    tres_id = tres_thread.get_pid();
    if(!tres_id){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(tres_thread.get_policy() != tres_policy){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(quatro_thread.create(thread_4,this,quatro_priority,quatro_policy)==-1){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!quatro_thread.is_valid()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    quatro_id = quatro_thread.get_pid();
    if(!quatro_id){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(quatro_thread.get_policy() != quatro_policy){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if (uno_thread.get_priority()!= uno_priority||
         dos_thread.get_priority()!= dos_priority||
         tres_thread.get_priority()!= tres_priority||
         quatro_thread.get_priority()!= quatro_priority){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    for(int i=0;i< Sched::get_priority_max(uno_policy);i++){
        uno_priority= (uno_priority==(Sched::get_priority_max(uno_policy)))?uno_priority:uno_priority+1;
        dos_priority= (dos_priority==Sched::get_priority_max(dos_policy))?dos_priority:dos_priority+1;
        tres_priority= (tres_priority==Sched::get_priority_max(tres_policy))?tres_priority:tres_priority+1;
        quatro_priority= (quatro_priority==Sched::get_priority_max(quatro_policy))?quatro_priority:quatro_priority+1;
        uno_thread.set_priority(uno_priority);
        dos_thread.set_priority(dos_priority);
        tres_thread.set_priority(tres_priority);
        quatro_thread.set_priority(quatro_priority);

        if (uno_thread.get_priority()!= uno_priority||
            dos_thread.get_priority()!= dos_priority||
            tres_thread.get_priority()!= tres_priority||
            quatro_thread.get_priority()!= quatro_priority){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
    }
    if(uno_priority!=Sched::get_priority_max(uno_policy)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;

    }
    if(uno_thread.is_running()||dos_thread.is_running()||\
          tres_thread.is_running()||quatro_thread.is_running()){
        Thread::yield();//self proccess
    }
    //join detach
    uno_thread.set_detachstate(PTHREAD_CREATE_DETACHED);
    if(uno_thread.get_detachstate()!=PTHREAD_CREATE_DETACHED){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(uno_thread.is_joinable()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    uno_thread.set_detachstate(PTHREAD_CREATE_JOINABLE);
    if(uno_thread.get_detachstate()==PTHREAD_CREATE_DETACHED){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!uno_thread.is_joinable()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //stack size
    if(!uno_thread.get_stacksize()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    dos_thread.set_detachstate(PTHREAD_CREATE_DETACHED);
    if(dos_thread.get_detachstate()!=PTHREAD_CREATE_DETACHED){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(dos_thread.is_joinable()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    dos_thread.set_detachstate(PTHREAD_CREATE_JOINABLE);
    if(dos_thread.get_detachstate()==PTHREAD_CREATE_DETACHED){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!dos_thread.is_joinable()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //stack size
    if(!uno_thread.get_stacksize()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(count_1 == 0||count_2==0||count_3==0||count_4 ==0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
//    void** temp;
//    pid_t current_pid;
    Timer timer_count;

//    current_pid = Sched::get_pid();
    timer_count.start();
    Thread::join(uno_thread);
//    uno_thread.wait(temp,1500);
//    print_case_message("timer after join %lu",timer_count.microseconds());
    //while before all thread is stoped
    while(uno_thread.is_running()||dos_thread.is_running()||\
          tres_thread.is_running()||quatro_thread.is_running()){
        Thread::yield();
    }
//    print_case_message("timer after join %lu",timer_count.microseconds());

    if((count_1 < 2)||(count_2<2)||(count_3<2)||(count_4<2)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //wait_time sets only in joined thread
    if(timer_count.microseconds()< wait_time){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //repeat create thread
    if(uno_thread.create(handle_thread_1,this,uno_priority,uno_policy)==-1){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(uno_thread.is_running()){
        uno_thread.kill(0);
    }else{
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //check after kill
    if(uno_thread.is_running()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    return result;

}
bool ThreadTest::execute_class_performance_case(){
    bool result = true;
    for (u16 i =0;i<1000;i++){

    }
    return result;
}
/* @brief stress test for sys/Thread use "api-sys-test -thread -api"
 * not writed: test add join
 *   thread start with priority bring check
 * not tested on :
 *  suspend if itterate_num >= 128
 */
bool ThreadTest::execute_class_stress_case(){
    bool result = true;
    const u32 itterate_num = 80;
    Thread uno_thread,dos_thread,tres_thread,quatro_thread;
    //pid_t uno_id,dos_id,tres_id,quatro_id;
    enum Sched::policy uno_policy,dos_policy,tres_policy,quatro_policy;
    u8 uno_priority=4;
    u8 dos_priority=4;
    u8 tres_priority =4;
    u8 quatro_priority =4;
    u8 max_prior_RR,max_prior_FIFO;
    //RR sheduler
    uno_policy = Sched::RR;
    dos_policy = Sched::RR;
    tres_policy = Sched::RR;
    quatro_policy = Sched::RR;
    Timer timer_count;
    max_prior_RR= Sched::get_priority_max(uno_policy);
    max_prior_FIFO= Sched::get_priority_max(dos_policy);
    for(u32 i=0;i<itterate_num;i++){
        u32 max_time;
        wait_time_quatro = ((u16)rand() & 0x1ff) +200;//used for quatro thread
        wait_time = ((u16)rand() & 0x1ff)+200;//used for thread uno dos tres
        max_time = wait_time>wait_time_quatro?wait_time:wait_time_quatro;//use max value
        max_time = wait_time;
        timer_count.start();
        count_1=0;count_2=0;count_3=0;count_4=0;
        if((uno_thread.create(handle_thread_1,this,uno_priority,uno_policy)==-1)||
            (dos_thread.create(handle_thread_2,this,dos_priority,dos_policy)==-1)||
            (tres_thread.create(handle_thread_3,this,tres_priority,tres_policy)==-1)||
            (quatro_thread.create(thread_4,NULL,quatro_priority,quatro_policy)==-1)){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
        uno_priority= ((i % max_prior_RR)==0?1:(i % max_prior_RR));
        uno_thread.set_priority(uno_priority);
        if (uno_thread.get_priority()!= uno_priority){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
        dos_priority= ((i % max_prior_FIFO)==0?1:(i % max_prior_FIFO));
        dos_thread.set_priority(dos_priority);
        if (dos_thread.get_priority()!= dos_priority){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }

        while(uno_thread.is_running()||dos_thread.is_running()||\
              tres_thread.is_running()||quatro_thread.is_running()){
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
    //FIFO sheduler
    uno_policy = Sched::FIFO;
    dos_policy = Sched::FIFO;
    tres_policy = Sched::FIFO;
    quatro_policy = Sched::FIFO;
    for(u32 i=0;i<itterate_num;i++){
        u32 max_time;
        wait_time_quatro = ((u16)rand() & 0x1ff) +200;//used for quatro thread
        wait_time = ((u16)rand() & 0x1ff)+200;//used for thread uno dos tres
        max_time = wait_time>wait_time_quatro?wait_time:wait_time_quatro;//use max value
        max_time = wait_time;
        timer_count.start();
        count_1=0;count_2=0;count_3=0;count_4=0;
        if((uno_thread.create(handle_thread_1,this,uno_priority,uno_policy)==-1)||
            (dos_thread.create(handle_thread_2,this,dos_priority,dos_policy)==-1)||
            (tres_thread.create(handle_thread_3,this,tres_priority,tres_policy)==-1)||
            (quatro_thread.create(thread_4,NULL,quatro_priority,quatro_policy)==-1)){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
        uno_priority= ((i % max_prior_RR)==0?1:(i % max_prior_RR));
        uno_thread.set_priority(uno_priority);
        if (uno_thread.get_priority()!= uno_priority){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
        dos_priority= ((i % max_prior_FIFO)==0?1:(i % max_prior_FIFO));
        dos_thread.set_priority(dos_priority);
        if (dos_thread.get_priority()!= dos_priority){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }

        while(uno_thread.is_running()||dos_thread.is_running()||\
              tres_thread.is_running()||quatro_thread.is_running()){
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
    for(u32 i=0;i<itterate_num;i++){
        u32 max_time;
        wait_time_quatro = ((u16)rand() & 0x1ff) +200;//used for quatro thread
        wait_time = ((u16)rand() & 0x1ff)+200;//used for thread uno dos tres
        max_time = wait_time>wait_time_quatro?wait_time:wait_time_quatro;//use max value
        max_time = wait_time;
        timer_count.start();
        count_1=0;count_2=0;count_3=0;count_4=0;
        if((uno_thread.create(handle_thread_1,this,uno_priority,uno_policy)==-1)||
            (dos_thread.create(handle_thread_2,this,dos_priority,dos_policy)==-1)||
            (tres_thread.create(handle_thread_3,this,tres_priority,tres_policy)==-1)||
            (quatro_thread.create(thread_4,NULL,quatro_priority,quatro_policy)==-1)){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
        uno_priority= ((i % max_prior_RR)==0?1:(i % max_prior_RR));
        uno_thread.set_priority(uno_priority);
        if (uno_thread.get_priority()!= uno_priority){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
        dos_priority= ((i % max_prior_FIFO)==0?1:(i % max_prior_FIFO));
        dos_thread.set_priority(dos_priority);
        if (dos_thread.get_priority()!= dos_priority){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }

        while(uno_thread.is_running()||dos_thread.is_running()||\
              tres_thread.is_running()||quatro_thread.is_running()){
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
    return result;
}

void * ThreadTest::thread_1(u32 wait_time){
    count_1++;
    Timer::wait_microseconds(wait_time);
    count_1++;
    return &count_1;
}
void * ThreadTest::thread_2(u32 wait_time){
    count_2++;
    Timer::wait_microseconds(wait_time);
    count_2++;
    return &count_2;
}
void * ThreadTest::thread_3(u32 wait_time){
    count_3++;
    Timer::wait_microseconds(wait_time);
    count_3++;
    return &count_3;
}

static void * thread_4(void * args){
    //ThreadTest * thread_test= (ThreadTest*)args;
    count_4++;
    Timer::wait_microseconds(wait_time_quatro);
    count_4++;
    return &count_4;
}
