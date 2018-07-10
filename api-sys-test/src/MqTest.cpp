#include <sapi/chrono.hpp>
#include "MqTest.hpp"
static char mq_name_thread[] = "dialog";
static char mq_name_thread_answer[] = "dialog_answer";

static char message_buff[64] = "clean";
const char message_thread_1[] = "bon jour";
const char message_thread_2[] = "ola";
const char message_thread_3[] = "verra";
const char message_thread_4[] = "hasta la vista";

static u32 count_0 = 0;
static u32 count_1 = 0;
static void * thread_1(void * args);
static Mutex mutex_test;


MqTest::MqTest():Test("sys::Mq"){

}
/*@brief api test for sys/Mq use "api-sys-test -mq -api"
 * Mq
 * Mq(),create(),open(),int get_attr(),MqAttr get_attr(),set_attr(),
 * MqAttr
 * MqAttr(),MqAttr(long f, long m, long s),
 * flags(),curmsgs(),maxmsg(),msgsize(),
 * set_flags(long v),set_curmsgs(long v),set_maxmsg(long v),set_msgsize(long v)
 * send(),receive()
 * not writed test
 * ,close(),is_open(),notify(),,
 * set_attr()
 * unlink(),msg_prio(),
 * @todo more api test for size and flags
 * @todo more stress test for size prio and flags
 * not tested on
 * @bug crashes out(suspend) if open() use before create() and with out mq_attr
 * @warning change mq_attr after close mq
 * @warning receive_timed crashes out(susoend) if use incorrect abs_time
 * @warning Mq class dont have O_RDWR
 * @warning stress test suspended
 */
bool MqTest::execute_class_api_case(){
    bool result = true;
    MqAttr mq_attr,mq_attr_get;
    struct mq_attr mq_struct;
    struct timespec *timeout;
    ClockTime clock_time,clock_time_add;
    Mq mq;
    long attr_flag,attr_mq_maxmsg,attr_msgsize,attr_curmsgs;
    int mq_oflag;
    char mq_name_uno[] = "common";
    mode_t mode = 0666;
    if(!execute_api_mq_attr_case(&mq_attr)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!execute_api_simply_case()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    mq_oflag = Mq::CREATE;
    attr_flag = MqAttr::RDWR;
    attr_mq_maxmsg = 10;
    attr_msgsize = 10;
    attr_curmsgs = 10;
    mq_attr.set_curmsgs(attr_curmsgs);
    mq_attr.set_flags(attr_flag);
    mq_attr.set_maxmsg(attr_mq_maxmsg);
    mq_attr.set_msgsize(attr_msgsize);
    mq_oflag = O_RDWR;
    if (mq.create(mq_name_uno,mq_oflag,mode,attr_flag,attr_mq_maxmsg,attr_msgsize)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    mq.set_attr(mq_attr);
    mq_attr_get = mq.get_attr();
    if(mq.get_attr(mq_struct)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq_attr_get.flags() != mq_attr.flags()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq_attr_get.curmsgs() != mq_attr.curmsgs()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq_attr_get.maxmsg() != mq_attr.maxmsg()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq_attr_get.msgsize() != mq_attr.msgsize()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    if(mq_struct.mq_flags != mq_attr.flags()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq_struct.mq_curmsgs != mq_attr.curmsgs()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq_struct.mq_maxmsg != mq_attr.maxmsg()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq_struct.mq_msgsize != mq_attr.msgsize()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq.close()<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //open after close

    if(mq.open(mq_name_uno,mq_oflag,mode,&mq_struct)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //change attr after close mq
    attr_flag = MqAttr::RDWR;
    attr_mq_maxmsg = 10;
    attr_msgsize = 16;
    attr_curmsgs = 16;
    mq_attr.set_curmsgs(attr_curmsgs);
    mq_attr.set_flags(attr_flag);
    mq_attr.set_maxmsg(attr_mq_maxmsg);
    mq_attr.set_msgsize(attr_msgsize);
    mq.set_attr(mq_attr);
    mq_attr_get = mq.get_attr();
    mq.get_attr(mq_struct);
    if(mq_attr_get.flags()!= mq_attr.flags()||\
       mq_attr_get.curmsgs() != mq_attr.curmsgs()||\
       mq_attr_get.maxmsg()  != mq_attr.maxmsg()||\
       mq_attr_get.msgsize() != mq_attr.msgsize()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    char message_uno[] = "uno";
    int len_mess;
    clock_time = Clock::get_time();
    clock_time_add.set(0,10*1000*1000);
    clock_time +=clock_time_add;
    timeout = clock_time.timespec();
    if(mq.send_timed(message_uno,sizeof(message_uno),0,timeout)>0){
        clock_time = Clock::get_time();
        clock_time +=clock_time_add;
        timeout = clock_time.timespec();
        len_mess = mq.receive_timed(message_uno,sizeof(message_uno),timeout);
        if(len_mess != sizeof(message_uno)){
            print_case_message("Failed %s:%d:%d", __PRETTY_FUNCTION__, __LINE__,len_mess );
            result = false;
        }
    }else{
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    return result;
}
/*  called in MqTest::execute_class_api_case(){
 * */
bool MqTest::execute_api_simply_case(){
    bool result =true;
    MqAttr mq_attr;
    struct timespec *timeout;
    ClockTime clock_time,clock_time_add;
    Mq mq;
    long attr_flag,attr_mq_maxmsg,attr_msgsize,attr_curmsgs;
    int mq_oflag;
    int len_mess;
    attr_flag = MqAttr::RDWR;
    mq_oflag = O_RDWR;
    mode_t mode = 666;
    attr_mq_maxmsg = 16;
    attr_msgsize = 16;
    attr_curmsgs = 16;
    char mq_name_uno[] = "simply";
    const char message_uno[] = "uno";
    const char message_dos[] = "bon jour";
    char message_buff[64] = "clean";
    mq_attr.set_curmsgs(attr_curmsgs);
    mq_attr.set_flags(attr_flag);
    mq_attr.set_maxmsg(attr_mq_maxmsg);
    mq_attr.set_msgsize(attr_msgsize);
    if (mq.create(mq_name_uno,mq_oflag,mode,attr_flag,attr_mq_maxmsg,attr_msgsize)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    clock_time = Clock::get_time();
    clock_time_add.set(0,10*1000*1000);
    clock_time +=clock_time_add;
    timeout = clock_time.timespec();
    if(mq.send_timed(message_uno,sizeof(message_uno),0,timeout)>0){
        clock_time = Clock::get_time();
        clock_time += clock_time_add;
        timeout = clock_time.timespec();
        len_mess = mq.receive_timed(message_buff,sizeof(message_uno),timeout);
        if(len_mess != sizeof(message_uno)){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
    }else{
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq.send(message_dos,sizeof(message_dos),0)>0){
        len_mess = mq.receive(message_buff,sizeof(message_dos));
        if(len_mess != sizeof(message_dos)){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
    }else{
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    return result;
}

/*  called in MqTest::execute_class_api_case(){
 * */
bool MqTest::execute_api_mq_attr_case(MqAttr * mq_attr){
    bool result = true;
    long attr_flag,attr_mq_maxmsg,attr_msgsize,attr_curmsgs;
    MqAttr mq_uno;
    attr_flag = MqAttr::NONBLOCK;
    attr_mq_maxmsg = 5;
    attr_msgsize = 5;
    MqAttr mq_dos(attr_flag ,attr_mq_maxmsg ,attr_msgsize);
    if(mq_dos.flags()!=attr_flag || mq_dos.maxmsg()!= attr_mq_maxmsg ||\
            mq_dos.msgsize()!=attr_msgsize){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    attr_flag = MqAttr::RDWR;
    attr_mq_maxmsg = 6;
    attr_msgsize = 12;
    mq_uno.set_flags(attr_flag);
    mq_uno.set_maxmsg(attr_mq_maxmsg);
    mq_uno.set_msgsize(attr_msgsize);
    if(mq_uno.flags()!=attr_flag || mq_uno.maxmsg()!= attr_mq_maxmsg ||\
            mq_uno.msgsize()!=attr_msgsize){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    attr_flag = MqAttr::RDONLY;
    mq_uno.set_flags(attr_flag);
    if(mq_uno.flags()!=attr_flag){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    attr_curmsgs = 1;
    mq_uno.set_curmsgs(attr_curmsgs);
    if(mq_uno.curmsgs()!=attr_curmsgs){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    mq_attr->set_curmsgs(attr_curmsgs);
    mq_attr->set_flags(attr_flag);
    mq_attr->set_maxmsg(attr_mq_maxmsg);
    mq_attr->set_msgsize(attr_msgsize);
    return result;
}
bool MqTest::execute_class_stress_case(){
    bool result = true;
    MqAttr mq_attr;
    Mq mq_answer,mq;
    long attr_flag,attr_mq_maxmsg,attr_msgsize,attr_curmsgs;
    int mq_oflag;
    mode_t mode = 666;
    Thread uno_thread;
    u32 itterate = 100;
    int len_mess;
    attr_flag = MqAttr::RDWR;
    attr_mq_maxmsg = 20;
    attr_msgsize = 16;
    attr_curmsgs = 16;
    mq_oflag = O_RDWR;
    mq_attr.set_curmsgs(attr_curmsgs);
    mq_attr.set_flags(attr_flag);
    mq_attr.set_maxmsg(attr_mq_maxmsg);
    mq_attr.set_msgsize(attr_msgsize);
    if (mq.create(mq_name_thread,mq_oflag,mode,attr_flag,attr_mq_maxmsg,attr_msgsize)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if((uno_thread.create(thread_1,this,1,Sched::RR)==-1)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    Timer::wait_microseconds(10000);
    Sched::yield();
    Timer::wait_microseconds(10000);
    count_0++;
    if(mq_answer.open(mq_name_thread_answer,mq_oflag)>=0){
        print_case_message("open answer");
        for (u32 i =0;i<itterate;i++){
            if(mq.send(message_thread_1,sizeof(message_thread_1),0)>0){
                print_case_message("sent");
                Sched::yield();
                Timer::wait_microseconds(5000);
                len_mess = mq_answer.receive(message_buff,sizeof(message_thread_2));
                if((len_mess == sizeof(message_thread_2)) && \
                        (strcmp(message_buff,message_thread_2)==0)){
                    print_case_message("receive answer");
                }
            }else{
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
        }
    }else{
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    if(mq.send(message_thread_3,sizeof(message_thread_3),0)>0){
        Timer::wait_microseconds(10000);
    }else{
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    return result;
}
bool MqTest::execute_class_performance_case(){
    bool result = true;
    return result;
}

static void * thread_1(void * args){
    Mq mq;
    Mq mq_answer;
    int mq_oflag = O_RDWR;
    int len_mess;
    mode_t mode = 666;
    (void) args;
    if (mq_answer.create(mq_name_thread_answer,mq_oflag,mode,MqAttr::RDWR\
                         ,20,16)>=0){
        if(mq.open(mq_name_thread,mq_oflag)>=0){
            while(1){
                count_1++;
                len_mess = mq.receive(message_buff,sizeof(message_thread_1));
                if((len_mess == sizeof(message_thread_1)) && \
                        (strcmp(message_buff,message_thread_1)==0)){
                    mq_answer.send(message_thread_2,sizeof(message_thread_2),0);
                }else if((len_mess == sizeof(message_thread_3)) &&\
                         (strcmp(message_buff,message_thread_3)==0)){
                    mq_answer.send(message_thread_4,sizeof(message_thread_4),0);
                    break;
                }
            }
        }
    }

    return &count_1;
}
