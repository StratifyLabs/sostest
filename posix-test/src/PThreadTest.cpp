#include "PThreadTest.hpp"

#include <sapi/sys.hpp>

static pthread_mutex_t thread_mutex;


PThreadTest::PThreadTest() : Test("posix::pthread"){
    m_count = 0;
}


bool PThreadTest::execute_class_api_case(){
    bool result = true;
    int i;

    pthread_mutexattr_t attr;
    pthread_t t;
    pthread_attr_t pattr;
    void * ret_value;

    int total_tasks_to_test = get_available_task_count();
    int total_large_tasks = total_tasks_to_test/2;

    print_case_message("Total tasks available %d", total_tasks_to_test);

    if ( pthread_mutexattr_init(&attr) < 0 ){
        print_case_message("failed to init attr (%d)", errno);
        return false;
    }


    if ( pthread_mutex_init(&thread_mutex, &attr) < 0 ){
        print_case_message("failed to init thread_mutex (%d)", errno);
        return false;
    }

    print_case_message("Test pthread_create().....");

    for(i=0; i < total_large_tasks; i++){

        print_case_message("Create memory for task %d of %d", i+1,total_tasks_to_test);

        if ( pthread_attr_init(&pattr) < 0 ){
            print_case_message("failed %s: %d", __FILE__, __LINE__);
            return false;
        }

        if ( i < (total_large_tasks - 1) ){
            if ( pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_DETACHED) < 0 ){
                print_case_message("failed %s: %d", __FILE__, __LINE__);
                return false;
            }
        } else {
            if ( pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_JOINABLE) < 0 ){
                print_case_message("failed %s: %d", __FILE__, __LINE__);
                return false;
            }
        }

        if ( pthread_attr_setstacksize(&pattr, 2048) < 0 ){
            print_case_message("failed %s: %d", __FILE__, __LINE__);
            return false;
        }

        m_count = i;
        if ( pthread_create(&t, &pattr, handle_thread0, this) < 0 ){
            print_case_message("failed %s: %d", __FILE__, __LINE__);
            return false;
        }
    }

    if ( pthread_join(t, NULL) < 0 ){
        print_case_message("failed %s: %d", __FILE__, __LINE__);
        return false;
    }

    print_case_message("passed");
    print_case_message("Test pthread_create() (small)...");

    for(i=0; i < total_tasks_to_test; i++){
        if ( pthread_attr_init(&pattr) < 0 ){
            print_case_message("failed %s: %d", __FILE__, __LINE__);
            return false;
        }

        if ( i < (total_tasks_to_test - 1) ){
            if ( pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_DETACHED) < 0 ){
                print_case_message("failed %s: %d", __FILE__, __LINE__);
                return false;
            }
        } else {
            if ( pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_JOINABLE) < 0 ){
                print_case_message("failed %s: %d", __FILE__, __LINE__);
                return false;
            }
        }

        if ( pthread_attr_setstacksize(&pattr, 384) < 0 ){
            print_case_message("failed %s: %d", __FILE__, __LINE__);
            return false;
        }


        if ( pthread_create(&t, &pattr, handle_thread1, this) ){
            print_case_message("failed %s: %d", __FILE__, __LINE__);
            return false;
        }
    }


    if ( pthread_join(t, NULL) < 0 ){
        print_case_message("failed %s: %d", __FILE__, __LINE__);
        return false;
    }

    print_case_message("passed");

    print_case_message("Test pthread_join()...");
    for(i=0; i < total_tasks_to_test; i++){

        if ( pthread_attr_init(&pattr) < 0 ){
            print_case_message("failed %s: %d", __FILE__, __LINE__);
            return false;
        }

        if ( pthread_attr_setdetachstate(&pattr, PTHREAD_CREATE_JOINABLE) < 0 ){
            print_case_message("failed %s: %d", __FILE__, __LINE__);
            return false;
        }

        if ( pthread_attr_setstacksize(&pattr, 384) < 0 ){
            print_case_message("failed %s: %d", __FILE__, __LINE__);
            return false;
        }

        if ( pthread_create(&t, &pattr, handle_thread1, this) ){
            print_case_message("failed %s: %d", __FILE__, __LINE__);
            return false;
        }

        if ( pthread_join(t, &ret_value) < 0 ){
            print_case_message("failed %s: %d", __FILE__, __LINE__);
            return false;
        }

        print_case_message("r%d...", (int)ret_value);

        if ( (int)ret_value != m_count ){
            print_case_message("failed to get return value (%d != %d)", (int)ret_value, m_count);
            return false;
        }
    }


    return result;
}

void * PThreadTest::thread0(){
    if ( pthread_mutex_lock(&thread_mutex) < 0 ){
        print_case_message("t%d failed to lock mutex (%d)", m_count, errno);
    }
    print_case_message("t%d", m_count++);
    usleep(700*1000);
    if ( pthread_mutex_unlock(&thread_mutex) < 0 ){
        print_case_message("t%d failed to lock mutex (%d)", m_count, errno);
    }
    return NULL;
}


void * PThreadTest::thread1(){
    pthread_mutex_lock(&thread_mutex);
    usleep(200*1000);
    pthread_mutex_unlock(&thread_mutex);
    return (void*)m_count;
}

int PThreadTest::get_available_task_count(){
	 TaskManager task;
    return task.count_free();
}

