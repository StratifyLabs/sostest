```
Test state 
    Writed
        Fifo 
            api 
                @warning non zero value after flush
        QSPI
            api
                sl.exe terminal.run:while=api-hal-test app.install:p=api-hal-test,run,args="-api -qspi"
            stress
                sl.exe terminal.run:while=api-hal-test app.install:p=api-hal-test,run,args="-stress -qspi"
        Emc
            api
                sl.exe terminal.run:while=api-hal-test app.install:p=api-hal-test,run,args="-api -emc"
            stress
                sl.exe terminal.run:while=api-hal-test app.install:p=api-hal-test,run,args="-stress -emc"

    Not Writed (skeleton)
```

