
#include <sapi/var.hpp>
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
 * @warning is_valid == true before created threads
 */
bool ThreadTest::execute_class_api_case(){
    bool result = true;
    Thread uno_thread(4096),dos_thread(1024),tres_thread(1024),quatro_thread(1024);
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
    Timer timer_count;

    if(uno_thread.get_detachstate()!=Thread::DETACHED){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(uno_thread.is_joinable()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    //join detach
    if( uno_thread.set_detachstate(Thread::DETACHED) < 0 ){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(uno_thread.get_detachstate()!=Thread::DETACHED){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(uno_thread.is_joinable()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if( uno_thread.set_detachstate(Thread::JOINABLE) < 0 ){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(uno_thread.get_detachstate()==Thread::DETACHED){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!uno_thread.is_joinable()){
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
    //repeat -- should fail before thread is busy
    if( !(uno_thread.create(handle_thread_1,this,uno_priority+1,uno_policy) < 0) ){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    if( dos_thread.is_valid() == true ){ //should be invalid before creating
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    if( dos_thread.set_detachstate(Thread::DETACHED) < 0 ){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(dos_thread.get_detachstate()!=Thread::DETACHED){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(dos_thread.is_joinable()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if( dos_thread.set_detachstate(Thread::JOINABLE) < 0 ){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(dos_thread.get_detachstate()==Thread::DETACHED){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!dos_thread.is_joinable()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    timer_count.start();
    if(dos_thread.create(handle_thread_2,this,dos_priority,dos_policy)==-1){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    if( dos_thread.is_valid() == false ){ //should be valid after creating
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
    print_case_message("Priority test");
    for(int i=0;i< Sched::get_priority_max(uno_policy);i++){
        uno_priority= (uno_priority==(Sched::get_priority_max(uno_policy)))?uno_priority:uno_priority+1;
        dos_priority= (dos_priority==Sched::get_priority_max(dos_policy))?dos_priority:dos_priority+1;
        tres_priority= (tres_priority==Sched::get_priority_max(tres_policy))?tres_priority:tres_priority+1;
        quatro_priority= (quatro_priority==Sched::get_priority_max(quatro_policy))?quatro_priority:quatro_priority+1;
        uno_thread.set_priority(uno_priority);
        dos_thread.set_priority(dos_priority);
        tres_thread.set_priority(tres_priority);
        quatro_thread.set_priority(quatro_priority);

        if( uno_thread.get_priority() != uno_priority ){
            print_case_message("Failed %s:%d (%d != %d)", __PRETTY_FUNCTION__, __LINE__, uno_thread.get_priority(), uno_priority);
            result = false;
        }

        if( dos_thread.get_priority() != dos_priority ){
            print_case_message("Failed %s:%d (%d != %d)", __PRETTY_FUNCTION__, __LINE__, dos_thread.get_priority(), dos_priority);
            result = false;
        }

        if( tres_thread.get_priority() != tres_priority ){
            print_case_message("Failed %s:%d (%d != %d) %d", __PRETTY_FUNCTION__, __LINE__, tres_thread.get_priority(), tres_priority, tres_thread.error_number());
            result = false;
        }

        if( quatro_thread.get_priority() != quatro_priority ){
            print_case_message("Failed %s:%d (%d != %d) %d", __PRETTY_FUNCTION__, __LINE__, quatro_thread.get_priority(), quatro_priority, quatro_thread.error_number());
            result = false;
        }
    }

    if(uno_priority!=Sched::get_priority_max(uno_policy)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;

    }

    print_case_message("Yield to threads");
    if(uno_thread.is_running()||dos_thread.is_running()||\
            tres_thread.is_running()||quatro_thread.is_running()){
        Thread::yield();//self proccess
    }

    //stack size
    if(!uno_thread.get_stacksize()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    //stack size
    if(!uno_thread.get_stacksize()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(count_1 == 0 || count_2==0 || count_3==0 || count_4 ==0){
        print_case_message("Failed %s:%d (%d,%d,%d,%d)", __PRETTY_FUNCTION__, __LINE__, count_1, count_2, count_3, count_4);
        result = false;
    }
    //    void** temp;
    //    pid_t current_pid;

    //    current_pid = Sched::get_pid();
    print_case_message("Join uno");
    //Thread::join(uno_thread);

    if( uno_thread.join() < 0 ){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    print_case_message("Join dos");

    if( Thread::join(dos_thread) < 0 ){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
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
        print_case_message("Failed %s:%d (%d < %d)", __PRETTY_FUNCTION__, __LINE__, timer_count.microseconds(), wait_time);
        result = false;
    }

    print_case_message("Repeat create uno");

    //repeat create thread
    if( uno_thread.set_detachstate(Thread::DETACHED) < 0 ){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if( uno_thread.create(handle_thread_1,this,uno_priority,uno_policy) < 0 ){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

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

    uno_thread.wait(0, 100);
    //check after kill
    if( uno_thread.is_running() ){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

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
    const u32 itterate_num = 1024;
    Thread uno_thread,dos_thread,tres_thread,quatro_thread;
    //pid_t uno_id,dos_id,tres_id,quatro_id;
    enum Sched::policy policy;
    u8 priority=4;
    u8 max_prior;
    //RR sheduler
    Timer timer_count;
    for(u32 i=0;i<itterate_num;i++){
        u32 max_time;
        timer_count.start();
        count_1=0;count_2=0;count_3=0;count_4=0;
        max_time = wait_time>wait_time_quatro?wait_time:wait_time_quatro;//use max value
        policy  = test_get_policy(i%3);
        max_prior= Sched::get_priority_max(policy);
        priority= ((i % max_prior)==0?1:(i % max_prior));
        if((uno_thread.create(handle_thread_1,this,priority,policy)==-1)||
                (dos_thread.create(handle_thread_2,this,priority,policy)==-1)||
                (tres_thread.create(handle_thread_3,this,priority,policy)==-1)||
                (quatro_thread.create(thread_4,NULL,priority,policy)==-1)){
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
        uno_thread.wait(0, 10);
        dos_thread.wait(0, 10);
        tres_thread.wait(0, 10);
        quatro_thread.wait(0, 10);
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
    const u32 itterate_num = 512;
    Thread uno_thread,dos_thread,tres_thread,quatro_thread;
    //pid_t uno_id,dos_id,tres_id,quatro_id;
    enum Sched::policy uno_policy,dos_policy,tres_policy,quatro_policy;
    u8 uno_priority=4;
    u8 dos_priority=4;
    u8 tres_priority =4;
    u8 quatro_priority =4;
    u8 max_prior_RR,max_prior_FIFO,max_prior_OTHER;
    //RR sheduler
    uno_policy = Sched::RR;
    dos_policy = Sched::RR;
    tres_policy = Sched::RR;
    quatro_policy = Sched::RR;
    Timer timer_count;
    max_prior_RR= Sched::get_priority_max(Sched::RR);
    max_prior_FIFO= Sched::get_priority_max(Sched::FIFO);
    max_prior_OTHER= Sched::get_priority_max(Sched::OTHER);
    for(u32 i=0;i<itterate_num;i++){
        u32 max_time;
        wait_time_quatro = ((u16)rand() & 0x1ff) +200;//used for quatro thread
        wait_time = ((u16)rand() & 0x1ff)+200;//used for thread uno dos tres
        max_time = wait_time>wait_time_quatro?wait_time:wait_time_quatro;//use max value
        timer_count.start();
        count_1=0;count_2=0;count_3=0;count_4=0;
        uno_priority= (u8)rand() % max_prior_RR;
        uno_priority= uno_priority==0?1:uno_priority;
        dos_priority= (u8)rand() % max_prior_RR;
        dos_priority= dos_priority==0?1:dos_priority;
        tres_priority= (u8)rand() % max_prior_RR;
        tres_priority= tres_priority==0?1:tres_priority;
        quatro_priority= (u8)rand() % max_prior_RR;
        quatro_priority= quatro_priority==0?1:quatro_priority;

        if((uno_thread.create(handle_thread_1,this,uno_priority,uno_policy)==-1)||
                (dos_thread.create(handle_thread_2,this,dos_priority,dos_policy)==-1)||
                (tres_thread.create(handle_thread_3,this,tres_priority,tres_policy)==-1)||
                (quatro_thread.create(thread_4,NULL,quatro_priority,quatro_policy)==-1)){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }

        if( uno_thread.set_priority(uno_priority) < 0 ){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
        if (uno_thread.get_priority()!= uno_priority){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }


        if( dos_thread.set_priority(dos_priority) < 0 ){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
        }

        if (dos_thread.get_priority()!= dos_priority){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }

        while(uno_thread.is_running()||dos_thread.is_running()||tres_thread.is_running()||quatro_thread.is_running()){
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

        uno_thread.wait(nullptr, 10);
        dos_thread.wait(nullptr, 10);
        tres_thread.wait(nullptr, 10);
        quatro_thread.wait(nullptr, 10);
        timer_count.stop();
    }
    //FIFO sheduler
    uno_policy = Sched::FIFO;
    dos_policy = Sched::FIFO;
    tres_policy = Sched::FIFO;
    quatro_policy = Sched::FIFO;
    for(u32 i=0;i<itterate_num;i++){
        u32 max_time;
        uno_priority= (u8)rand() % max_prior_FIFO;
        uno_priority= uno_priority==0?1:uno_priority;
        dos_priority= (u8)rand() % max_prior_FIFO;
        dos_priority= dos_priority==0?1:dos_priority;
        tres_priority= (u8)rand() % max_prior_FIFO;
        tres_priority= tres_priority==0?1:tres_priority;
        quatro_priority= (u8)rand() % max_prior_FIFO;
        quatro_priority= quatro_priority==0?1:quatro_priority;
        wait_time_quatro = ((u16)rand() & 0x1ff) +200;//used for quatro thread
        wait_time = ((u16)rand() & 0x1ff)+200;//used for thread uno dos tres
        max_time = wait_time>wait_time_quatro?wait_time:wait_time_quatro;//use max value
        timer_count.start();
        count_1=0;count_2=0;count_3=0;count_4=0;
        if((uno_thread.create(handle_thread_1,this,uno_priority,uno_policy)==-1)||
                (dos_thread.create(handle_thread_2,this,dos_priority,dos_policy)==-1)||
                (tres_thread.create(handle_thread_3,this,tres_priority,tres_policy)==-1)||
                (quatro_thread.create(thread_4,nullptr,quatro_priority,quatro_policy)==-1)){
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
        uno_priority= (u8)rand() % max_prior_FIFO;
        uno_priority= uno_priority==0?1:uno_priority;
        dos_priority= (u8)rand() % max_prior_RR;
        dos_priority= dos_priority==0?1:dos_priority;
        tres_priority= (u8)rand() % max_prior_FIFO;
        tres_priority= tres_priority==0?1:tres_priority;
        quatro_priority= (u8)rand() % max_prior_RR;
        quatro_priority= quatro_priority==0?1:quatro_priority;

        wait_time_quatro = ((u16)rand() & 0x1ff) +200;//used for quatro thread
        wait_time = ((u16)rand() & 0x1ff)+200;//used for thread uno dos tres
        max_time = wait_time>wait_time_quatro?wait_time:wait_time_quatro;//use max value
        timer_count.start();
        count_1=0;count_2=0;count_3=0;count_4=0;
        if((uno_thread.create(handle_thread_1,this,uno_priority,uno_policy)==-1)||
                (dos_thread.create(handle_thread_2,this,dos_priority,dos_policy)==-1)||
                (tres_thread.create(handle_thread_3,this,tres_priority,tres_policy)==-1)||
                (quatro_thread.create(thread_4,nullptr,quatro_priority,quatro_policy)==-1)){
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
    uno_policy = Sched::OTHER;
    dos_policy = Sched::OTHER;
    tres_policy = Sched::OTHER;
    quatro_policy = Sched::OTHER;
    for(u32 i=0;i<itterate_num;i++){
        u32 max_time;
        uno_priority= (u8)rand() % max_prior_OTHER;
        uno_priority= uno_priority==0?1:uno_priority;
        dos_priority= (u8)rand() % max_prior_OTHER;
        dos_priority= dos_priority==0?1:dos_priority;
        tres_priority= (u8)rand() % max_prior_OTHER;
        tres_priority= tres_priority==0?1:tres_priority;
        quatro_priority= (u8)rand() % max_prior_OTHER;
        quatro_priority= quatro_priority==0?1:quatro_priority;
        wait_time_quatro = ((u16)rand() & 0x1ff) +200;//used for quatro thread
        wait_time = ((u16)rand() & 0x1ff)+200;//used for thread uno dos tres
        max_time = wait_time>wait_time_quatro?wait_time:wait_time_quatro;//use max value
        timer_count.start();
        count_1=0;count_2=0;count_3=0;count_4=0;
        if((uno_thread.create(handle_thread_1,this,uno_priority,uno_policy)==-1)||
                (dos_thread.create(handle_thread_2,this,dos_priority,dos_policy)==-1)||
                (tres_thread.create(handle_thread_3,this,tres_priority,tres_policy)==-1)||
                (quatro_thread.create(thread_4,nullptr,quatro_priority,quatro_policy)==-1)){
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
    if(tread_result == false){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    return result;
}

void * ThreadTest::thread_1(u32 wait_time){
    Queue <u8>u8_queue;
    Queue <u32>u32_queue;
    Queue <double>double_queue;
    count_1++;
    Timer::wait_microseconds(wait_time);
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
