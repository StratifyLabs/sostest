```
Test state 
    Writed
        Scheduler test
            -api passed
            -stress failed
                "msg-0": "Failed in cycle virtual bool SchedTest::execute_class_stress_case():224:1"
            -performance failed 
                "msg-0": "Failed in cycle virtual bool SchedTest::execute_class_performance_case():298:1"
            @warning set sheduller with OTHER param
            @todo add test for close waiting thread(wait mutex or messege)
        Thread test
            -api failed 
                "msg-2": "Failed virtual bool ThreadTest::execute_class_api_case():222 (-1 != 15) 3",
                "msg-3": "Failed virtual bool ThreadTest::execute_class_api_case():227 (-1 != 16) 3",
            -stress failed
            -performance failed
            @warning test failed, but before it passed(in prev version kernel)
        Mutex test 
            -api passed
            -stress passed
            -performance 2223953 uSec
        Mq test 
            -api passed
            -stress passed 
            -performance passed 
        Sys test 
            -api passed
            -stress passed 
            -performance passed 
        Signal test 
            -api passed
            -stress failed 
                "msg-0": "Failed virtual bool SignalTest::execute_class_stress_case():255"
            -performance failed
                "msg-0": "Failed virtual bool SignalTest::execute_class_performance_case():329"
            @warning test failed, but before it passed(in prev version kernel)
        Task test 
            -api passed
            -stress passed 
            -performance passed 
        Task cli 
            -api passed
            -stress passed 
            -performance passed 
    Not Writed (skeleton)
        Messenger cli 
            -api 
            -stress 
            -performance
            @son not created 
        Trace cli 
            -api 
            -stress 
            -performance 
            @???????
        Appfs cli 
            -api 
            -stress 
            -performance 
            @file not created
```

