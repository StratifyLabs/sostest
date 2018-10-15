#include <sapi/chrono.hpp>
#include <sapi/var.hpp>
#include "MqTest.hpp"
static char mq_name_thread[] = "dialog";
static char mq_name_thread_answer[] = "dialog_answer";

static char message_buff[64] = "clean";
const char message1_thread[] = "bon_jour";
const char message2_thread[] = "ola";
char message_rand[] = "123456789";
char message_rand_thread[] = "123456789";
const char message3_thread[] = "verra";
const char message4_thread[] = "hasta_la_vista";

static u32 count_0 = 0;
static u32 count_1 = 0;
static void * thread_1(void * args);
static Mutex mutex_test;
static Mq::flags get_o_flags(int i);
static void rand_string_value(u16 size,char * string);
MqTest::MqTest():Test("sys::Mq"){
	Data::reclaim_heap_space();
}
/*@brief api test for sys/Mq use "api-sys-test -mq -api"
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
    attr_msgsize = 12;
    attr_curmsgs = 0;
    mq_attr.set_curmsgs(attr_curmsgs);
    mq_attr.set_flags(attr_flag);
    mq_attr.set_maxmsg(attr_mq_maxmsg);
    mq_attr.set_msgsize(attr_msgsize);
    mq_oflag = O_RDWR;
    if(mq_attr.flags() != attr_flag){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq_attr.curmsgs() != attr_curmsgs){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        print_case_message("curmsgs %llu :%llu",mq_attr.curmsgs(),mq_attr_get.curmsgs());
        result = false;
    }
    if(mq_attr.maxmsg() != attr_mq_maxmsg){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq_attr.msgsize() != attr_msgsize){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if (mq.create(mq_name_uno,mq_oflag,mode,attr_flag,attr_mq_maxmsg,attr_msgsize)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq.set_attr(mq_attr)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
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
        print_case_message("curmsgs %llu :%llu",mq_attr.curmsgs(),mq_attr_get.curmsgs());
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
    attr_curmsgs = 0;
    mq_attr.set_flags(attr_flag);
    mq.set_attr(mq_attr);
    mq_attr_get = mq.get_attr();
    mq.get_attr(mq_struct);
    if(mq_attr_get.flags()!= mq_attr.flags()||
       mq_attr_get.maxmsg()  != mq_attr.maxmsg()||
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
    mq.unlink(mq_name_uno);
    mq.close();
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
    attr_curmsgs = 0;
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
    if(mq.unlink(mq_name_uno)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq.close()!=0){
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
    ClockTime clock_time,clock_time_add;
    struct timespec *timeout;

    int itterate = 1000;
    int len_mess;
    attr_flag = MqAttr::RDWR;
    attr_mq_maxmsg = 20;
    attr_msgsize = 16;
    attr_curmsgs = 16;

    mq_attr.set_curmsgs(attr_curmsgs);
    mq_attr.set_flags(attr_flag);
    mq_attr.set_maxmsg(attr_mq_maxmsg);
    mq_attr.set_msgsize(attr_msgsize);
    mq_oflag = Mq::CREATE;
    if (mq.create(mq_name_thread,mq_oflag,mode,attr_flag,attr_mq_maxmsg,attr_msgsize)<0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    if((uno_thread.create(thread_1,this,1,Sched::RR)==-1)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    count_0++;
    Timer::wait_milliseconds(1);
    if(mq_answer.open(mq_name_thread_answer,mq_oflag)>=0){
        for (int i =0;i<itterate;i++){
            if(mq.send(message1_thread,sizeof(message1_thread),0)>0){
                len_mess = mq_answer.receive(message_buff,sizeof(message_buff));
                if((len_mess == sizeof(message2_thread)) && \
                        (strncmp(message_buff,message2_thread,sizeof(message2_thread))==0)){

                }else{
                    print_case_message("Failed %s:%d:%s:%s", __PRETTY_FUNCTION__, __LINE__, message2_thread,message_buff);
                    result = false;
                    break;
                }
            }else{
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
        }
        for (int i =0;i<itterate;i++){
            clock_time = Clock::get_time();
            clock_time_add.set(0,10*1000*1000);
            clock_time +=clock_time_add;
            timeout = clock_time.timespec();
            if(mq.timedsend(message1_thread,sizeof(message1_thread),0,timeout)>0){
                len_mess = mq_answer.receive(message_buff,sizeof(message_buff));
                if((len_mess == sizeof(message2_thread)) && \
                        (strncmp(message_buff,message2_thread,sizeof(message2_thread))==0)){
                }else{
                    print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                    result = false;
                    break;
                }
            }else{
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
        }
        //use random messege
        rand_string_value(sizeof(message_rand),message_rand);
        for (int i =0;i<itterate;i++){
            clock_time = Clock::get_time();
            clock_time_add.set(0,10*1000*1000);
            clock_time +=clock_time_add;
            timeout = clock_time.timespec();

            if(mq.timedsend(message_rand,sizeof(message_rand),0,timeout)>0){
                len_mess = mq_answer.receive(message_buff,sizeof(message_buff));
                if((len_mess == sizeof(message_rand_thread)) && \
                        (strncmp(message_buff,message_rand_thread,sizeof(message_rand_thread))==0)){

                }else{
                    print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                    result = false;
                    break;
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

    if(mq.send(message3_thread,sizeof(message3_thread),0)>0){
    }else{
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    len_mess = mq_answer.receive(message_buff,sizeof(message4_thread));
    if((len_mess == sizeof(message4_thread)) && \
            (strcmp(message_buff,message4_thread)==0)){
    }else{
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq.unlink(mq_name_thread)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq.close()!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq_answer.unlink(mq_name_thread)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq_answer.close()!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    for (int i =0;i<itterate;i++){
        char mq_name_temp[10] = "abcdefghf";
        mq_oflag = get_o_flags(i);
        rand_string_value(sizeof(mq_name_temp)-1,mq_name_temp);
        if (mq.create(mq_name_temp,mq_oflag,mode,attr_flag,attr_mq_maxmsg,attr_msgsize)<0){
            print_case_message("Failed %s:%d:%d", __PRETTY_FUNCTION__, __LINE__,i);
            print_case_message("%s",mq_name_temp);
            result = false;
            break;
        }
        if(mq.unlink(mq_name_temp)!=0){
            print_case_message("Failed %s:%d:%d", __PRETTY_FUNCTION__, __LINE__,i);
            result = false;
            break;
        }
        if(mq.close()!=0){
            print_case_message("Failed %s:%d:%d", __PRETTY_FUNCTION__, __LINE__,i);
            result = false;
            break;
        }
    }
    return result;
}
bool MqTest::execute_class_performance_case(){
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
    attr_curmsgs = 0;
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
    count_0++;
    Timer::wait_milliseconds(1);
    if(mq_answer.open(mq_name_thread_answer,mq_oflag)>=0){
        for (u32 i =0;i<itterate;i++){
            if(mq.send(message1_thread,sizeof(message1_thread),0)>0){
                len_mess = mq_answer.receive(message_buff,sizeof(message2_thread));
                if((len_mess == sizeof(message2_thread)) && \
                        (strcmp(message_buff,message2_thread)==0)){

                }else{
                    print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                    result = false;
                    break;
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

    if(mq.send(message3_thread,sizeof(message3_thread),0)>0){
    }else{
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    len_mess = mq_answer.receive(message_buff,sizeof(message4_thread));
    if((len_mess == sizeof(message4_thread)) && \
            (strcmp(message_buff,message4_thread)==0)){
    }else{
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq.unlink(mq_name_thread)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq.close()!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq_answer.unlink(mq_name_thread)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(mq_answer.close()!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
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
                len_mess = mq.receive(message_buff,sizeof(message_buff));
                if((len_mess == sizeof(message1_thread)) && \
                        (strncmp(message_buff,message1_thread,sizeof(message1_thread))==0)){
                    mq_answer.send(message2_thread,sizeof(message2_thread),0);
                }else if((len_mess == sizeof(message3_thread)) &&\
                         (strncmp(message_buff,message3_thread,sizeof(message3_thread))==0)){
                    mq_answer.send(message4_thread,sizeof(message4_thread),0);
                    Timer::wait_milliseconds(100);
                    break;
                }else if(strncmp(message_buff,message_rand,sizeof(message_rand))==0){
                    rand_string_value(sizeof(message_rand_thread),message_rand_thread);
                    mq_answer.send(message_rand_thread,sizeof(message_rand_thread),0);
                }
            }
        }
    }
    return &count_1;
}
Mq::flags get_o_flags(int i){
    switch (i%3){
    case 0:
        return Mq::CREATE;
    case 1:
        return Mq::EXCL;
    case 2:
        return Mq::EXCLUSIVE;
    default:
        return Mq::CREATE;
    }
    return Mq::CREATE;
}
static void rand_string_value(u16 size,char * string){
    for (u16 i =0;i<size;i++){
        u8 value;
        value = (u8)(rand()%25);
        value +=97;
        string[i] = value;
    }
}
