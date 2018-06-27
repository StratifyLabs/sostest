
#include "SignalTest.hpp"

SignalTest::SignalTest() : Test("sys::Signal"){

}
static volatile bool wait_for_signal;
static void my_handler(int a);
static void my_handler(int a){
    //do something
    (void)a;
    wait_for_signal = false;
}

/*@brief api test for sys/Signal use "api-sys-test -signal -api"
 * SignalHandler
 *
 * Signal
 * set_handler,signo,sigvalue
 * not writed test
 * Signal,Signal,event,trigger,trigger,trigger,trigger,
 *
 * not tested on
 * suspend if signo = 0
 */
bool SignalTest::execute_class_api_case(){
    bool result = true;
    int signo,sig_value;
    SignalHandler handler(my_handler);
    signo = 0;
    sig_value = Signal::INT;
    Signal event(signo,sig_value);
    Timer timer;
    wait_for_signal = true;
    timer.start();
    //this will cause my_handler() to be executed when SIGINT is received
    if(event.signo()!=signo){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(event.set_handler(handler)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(event.sigvalue()!= sig_value){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //the program will stay in this loop until SIGINT is received
    while( wait_for_signal == true ){
        Timer::wait_msec(1);
        event.trigger();
    }
    print_case_message("timer %d",timer.microseconds());
    return result;
}
/*@brief stress test for sys/Signal use "api-sys-test -signal -stress"
 * not writed test
 * all sig value
 */
bool SignalTest::execute_class_stress_case(){
    bool result = true;
    int signo,sig_value;
    SignalHandler handler(my_handler);
    u16 itterate_num = 100;
    u8 itterate_signo_num = 0;
    u8 itterate_sig_value_num = 30;
    Timer timer;
    wait_for_signal = true;
    timer.start();
    //signo test
    for (int j=0;j<itterate_num;j++){
        for (int i=0;i<itterate_signo_num;i++){
            //this will cause my_handler() to be executed when SIGINT is received
            signo = i;
            sig_value = Signal::INT;
            Signal event(signo,sig_value);
            if(event.signo()!=signo){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
            if(event.sigvalue()!= sig_value){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
            if(event.set_handler(handler)!=0){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
            //the program will stay in this loop until SIGINT is received
            while( wait_for_signal == true ){
                Timer::wait_msec(1);
                event.trigger();
            }
            wait_for_signal = true;
        }
        //sig_value test
        for (int i=0;i<itterate_sig_value_num;i++){
            //this will cause my_handler() to be executed when SIGINT is received
            signo = 1;
            sig_value = i;
            Signal event(signo,sig_value);
            if(event.signo()!=signo){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
            if(event.sigvalue()!= sig_value){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
            if(event.set_handler(handler)!=0){
                print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
                result = false;
                break;
            }
            //the program will stay in this loop until SIGINT is received
            while( wait_for_signal == true ){
                Timer::wait_msec(1);
                event.trigger();
            }
            wait_for_signal = true;
        }
    }
    print_case_message("timer %d",timer.microseconds());
    return result;
}
/*@brief performance test for sys/Signal use "api-sys-test -signal -performance"
 */
bool SignalTest::execute_class_performance_case(){
    bool result = true;

    return result;
}
