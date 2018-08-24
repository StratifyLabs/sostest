#include <sapi/var.hpp>

#include "AdcTest.hpp"

AdcTest::AdcTest() : Test("hal::Adc"){

}


bool AdcTest::execute_class_api_case(){
    bool result = true;

    get_adc_count();

    print_case_message("Board has %d ADCs", m_adc_count);

    for(PeriphObject::port_t count = 0; count < m_adc_count; count++){
        Adc adc(count);
        if( execute_adc_api_case(adc) == false ){
            result = false;
        }
    }


    return result;
}

bool AdcTest::execute_adc_api_case(Adc & adc){
    bool result = true;
    int adc_result;
    String test_name;

    test_name.sprintf("ADC:%d", adc.port());

    open_case(test_name.str());

    if( adc.open() < 0 ){
        print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, adc.port());
        result = false;
    } else {

        adc_info_t info;

        if( adc.get_info(info) < 0 ){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, adc.port());
            result = false;
        } else {
            if( info.o_flags & Adc::SET_CONVERTER ){ print_case_message("Set Converter OK"); }
            if( info.o_flags & Adc::IS_LEFT_JUSTIFIED ){ print_case_message("Left justified OK"); }
            if( info.o_flags & Adc::IS_RIGHT_JUSTIFIED ){ print_case_message("Right justified OK"); }
            if( info.o_flags & Adc::IS_SCAN_MODE ){ print_case_message("Scan Mode OK"); }
            if( info.o_flags & Adc::IS_GROUP ){ print_case_message("Group OK"); }
            if( info.o_flags & Adc::IS_TRIGGER_EINT ){
                print_case_message("Trigger EINT OK");
                if( info.o_flags & Adc::IS_TRIGGER_EINT_EDGE_FALLING ){ print_case_message("Trigger EINT Falling OK"); }
                if( info.o_flags & Adc::IS_TRIGGER_EINT_EDGE_RISING ){ print_case_message("Trigger EINT Rising OK"); }
            }
            if( info.o_flags & Adc::IS_TRIGGER_TMR ){ print_case_message("Trigger TMR OK"); }


            adc_attr_t attr;

            attr.o_flags = (Adc::SET_CONVERTER | Adc::SET_CHANNELS);
            attr.freq = 0;
            attr.pin_assignment.channel[0] = mcu_pin(0, 3); //PA3 - AN0 - channel 3
            attr.pin_assignment.channel[1] = mcu_pin(2, 0); //PC0 - AN1 - channel 10
            attr.pin_assignment.channel[2] = mcu_pin(2, 3); //PC3 - AN2 - channel 13
            attr.pin_assignment.channel[3] = mcu_pin(5, 3); //PF3 - AN3 - channel 15 ??
            attr.channel_count = 1;
            attr.width = 12;

            if( (adc_result = adc.set_attr(attr)) < 0 ){
                print_case_message("Failed %s %d (%d, %d)", __FILE__, __LINE__, adc_result, adc.error_number());
                result = false;
            } else {

                memset(&attr.pin_assignment, 0xff, sizeof(attr.pin_assignment));
                attr.o_flags = Adc::SET_CHANNELS | Adc::IS_GROUP;
                attr.channel = 10;
                attr.sampling_time = 480;

                //for(attr.rank = 1; attr.rank <= 4; attr.rank++){
                attr.rank = 1;
                print_case_message("Configure channel %d as rank %d", attr.channel, attr.rank);
                if( (adc_result = adc.set_attr(attr)) < 0 ){
                    print_case_message("Failed %s %d (%d, %d)", __FILE__, __LINE__, adc_result, adc.error_number());
                    result = false;
                }
                //}

                attr.rank = 2;
                attr.channel = 3;
                print_case_message("Configure channel %d as rank %d", attr.channel, attr.rank);
                if( adc.set_attr(attr) < 0 ){
                    print_case_message("Failed %s %d (%d, %d)", __FILE__, __LINE__, adc_result, adc.error_number());
                    result = false;
                }

                attr.rank = 3;
                attr.channel = 10;
                print_case_message("Configure channel %d as rank %d", attr.channel, attr.rank);
                if( adc.set_attr(attr) < 0 ){
                    print_case_message("Failed %s %d (%d, %d)", __FILE__, __LINE__, adc_result, adc.error_number());
                    result = false;
                }

                attr.rank = 4;
                attr.channel = 3;
                print_case_message("Configure channel %d as rank %d", attr.channel, attr.rank);
                if( adc.set_attr(attr) < 0 ){
                    print_case_message("Failed %s %d (%d, %d)", __FILE__, __LINE__, adc_result, adc.error_number());
                    result = false;
                }

                if( result ){
                    Vector<u16> samples(64);
                    samples.fill(0);

                    attr.channel = 3;
                    attr.rank = 2;
                    adc.set_attr(attr);

                    adc.clear_error_number();
                    adc_result = adc.read(3, samples.data(), samples.count()*2);
                    if( adc_result < 0 ){
                        print_case_message("Failed %s %d (%d, %d)", __FILE__, __LINE__, adc_result, adc.error_number());
                        result = false;
                    } else {
                        for(u32 i = 0; i < samples.count(); i++){
                            print_case_message("%d Read 0x%X", i, samples.at(i));
                        }
                    }

                    attr.channel = 10;
                    attr.rank = 2;
                    adc.set_attr(attr);

                    adc.clear_error_number();
                    adc_result = adc.read(10, samples.data(), samples.count()*2);
                    if( adc_result < 0 ){
                        print_case_message("Failed %s %d (%d, %d)", __FILE__, __LINE__, adc_result, adc.error_number());
                        result = false;
                    } else {
                        for(u32 i = 0; i < samples.count(); i++){
                            print_case_message("%d Read 0x%X", i, samples.at(i));
                        }
                    }
                }
            }

        }

        if( adc.close() < 0 ){
            print_case_message("Failed %s %d: port:%d", __FILE__, __LINE__, adc.port());
            result = false;
        }
    }

    close_case(result);

    return result;
}

bool AdcTest::execute_class_performance_case(){
    bool result = true;

    return result;
}

bool AdcTest::execute_class_stress_case(){
    bool result = true;
    return result;
}

void AdcTest::get_adc_count(){
    int adc_count = 0;
    bool open_result = true;

    do {
        Adc adc(adc_count);

        if( adc.open() < 0 ){
            open_result = false;
        } else {
            adc_count++;
            adc.close();
        }

    } while( open_result );

    m_adc_count = adc_count;
}

