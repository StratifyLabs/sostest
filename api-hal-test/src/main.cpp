#include <stdio.h>

#include <sapi/sys.hpp>
#include <sapi/test.hpp>

#include "AdcTest.hpp"
#include "CoreTest.hpp"
#include "FifoTest.hpp"


//update flags
enum {
    //API, PERFORMANCE and STRESS are the lowest bits
    ADC_TEST_FLAG = (1<<5),
    CFIFO_TEST_FLAG = (1<<6),
    CORE_TEST_FLAG = (1<<7),
    DAC_TEST_FLAG = (1<<8),
    DEVICE_TEST_FLAG = (1<<9),
    DEVICE_SIGNAL_TEST_FLAG = (1<<10),
    DISPLAY_TEST_FLAG = (1<<11),
    DISPLAY_DEVICE_TEST_FLAG = (1<<12),
    EINT_TEST_FLAG = (1<<13),
    FIFO_TEST_FLAG = (1<<14),
    I2C_TEST_FLAG = (1<<15),
    LED_TEST_FLAG = (1<<16),
    PIN_TEST_FLAG = (1<<17),
    PIO_TEST_FLAG = (1<<18),
    PWM_TEST_FLAG = (1<<19),
    RTC_TEST_FLAG = (1<<20),
    SPI_TEST_FLAG = (1<<21),
    SWITCHBOARD_TEST_FLAG = (1<<22),
    TMR_TEST_FLAG = (1<<23),
    UART_TEST_FLAG = (1<<24),
    USB_TEST_FLAG = (1<<25)
};

u32 decode_cli(const Cli & cli, u32 & execute_flags);

int main(int argc, char * argv[]){
    Cli cli(argc, argv);
    cli.set_publisher("Stratify Labs, Inc");
    cli.handle_version();
    u32 o_flags;
    u32 o_execute_flags;

    o_flags = decode_cli(cli, o_execute_flags);

    Test::initialize(cli.name(), cli.version());

    if( o_flags & ADC_TEST_FLAG ){
        AdcTest test;
        test.execute(o_execute_flags);
    }


    if( o_flags & CORE_TEST_FLAG ){
        CoreTest test;
        test.execute(o_execute_flags);
    }

    if( o_flags & FIFO_TEST_FLAG ){
        FifoTest test;
        test.execute(o_execute_flags);
    }

    Test::finalize();
    return 0;

}

u32 decode_cli(const Cli & cli, u32 & execute_flags){
    u32 o_flags = 0;

    execute_flags = 0;

    if(cli.is_option("-all") ){
        o_flags = 0xffffffff;
        execute_flags |= Test::EXECUTE_ALL;
        return o_flags;
    }


    if(cli.is_option("-execute_all") ){ execute_flags |= Test::EXECUTE_ALL; }
    if(cli.is_option("-api") ){ execute_flags |= Test::EXECUTE_API; }
    if(cli.is_option("-stress") ){ execute_flags |= Test::EXECUTE_STRESS; }
    if(cli.is_option("-performance") ){ execute_flags |= Test::EXECUTE_PERFORMANCE; }
    if(cli.is_option("-additional") ){ execute_flags |= Test::EXECUTE_ADDITIONAL; }

    //update switches
    if(cli.is_option("-test_all") ){ o_flags = 0xffffffff; }
    if(cli.is_option("-adc") ){ o_flags |= ADC_TEST_FLAG; }
    if(cli.is_option("-cfifo") ){ o_flags |= CFIFO_TEST_FLAG; }
    if(cli.is_option("-core") ){ o_flags |= CORE_TEST_FLAG; }
    if(cli.is_option("-dac") ){ o_flags |= DAC_TEST_FLAG; }
    if(cli.is_option("-device") ){ o_flags |= DEVICE_TEST_FLAG; }
    if(cli.is_option("-fifo") ){ o_flags |= FIFO_TEST_FLAG; }

    return o_flags;
}



