#ifndef SIGNALTEST_HPP
#define SIGNALTEST_HPP

#include <sapi/test.hpp>
#include <sapi/dsp.hpp>
#include <sapi/chrono.hpp>

template<typename SignalType> class SignalTest : public Test {
public:
    SignalTest(const char * name) : Test(name){}

    bool execute_class_api_case(){
        bool result = true;
        u32 i;

		  print_case_message("create signal types");
        SignalType data(32);
        SignalType data1;
        SignalType data2;

		  print_case_message("check for api");
		  if( data.is_api_available() == false ){
			  print_case_failed("Api is not available");
			  return case_result();
		  }

		  print_case_message("check count");
        if( data.count() != 32 ){
            result = false;
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }

		  print_case_message("fill values");
        for(i=0; i < data.count(); i++){
            if( i % 2 ){
                data[i] = 10;
            } else {
                data[i] = -10;
            }
        }

        if( data.mean() != 0 ){
            result = false;
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }

        if( data.min() != -10 ){
            result = false;
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }

        if( data.max() != 10 ){
            result = false;
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }

        data1 = data.abs();
        if( data1.count() != 32 ){
            result = false;
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }

        if( data1.min() != 10 ){
            result = false;
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }

        if( data1.max() != 10 ){
            result = false;
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }

        if( data1.mean() != 10 ){
            result = false;
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        }

        data1 = data + data;
        data2 = data + data;

        if( data1.mean() != 0 ){
            result = false;
            print_case_message("Failed %s:%d (%ld)", __PRETTY_FUNCTION__, __LINE__, data1.mean());
        }

        if( data1.max() != 20 ){
            result = false;
            print_case_message("Failed %s:%d (%ld)", __PRETTY_FUNCTION__, __LINE__, data1.mean());
        }

        if( data1.min() != -20 ){
            result = false;
            print_case_message("Failed %s:%d (%ld)", __PRETTY_FUNCTION__, __LINE__, data1.mean());
        }

        if( data1 != data2 ){
            result = false;
            print_case_message("Failed %s:%d (%ld)", __PRETTY_FUNCTION__, __LINE__, data1.mean());
        }

        data2[0] = 0;

        if( data1 == data2 ){
            result = false;
            print_case_message("Failed %s:%d (%ld)", __PRETTY_FUNCTION__, __LINE__, data1.mean());
        }

        //data1 = data + data
        data2 = data1 - data;
        if( data2 != data ){
            result = false;
            print_case_message("Failed %s:%d (%ld)", __PRETTY_FUNCTION__, __LINE__, data1.mean());
        }

        print_case_message("done");

        return result;
    }

    bool execute_class_performance_case(){

        return true;
    }


    bool execute_class_stress_case(){
        return true;
    }

    virtual bool is_api_available(){
        return false;
    }

};

class SignalQ15Test : public SignalTest<SignalQ15> {
public:
    SignalQ15Test() : SignalTest("dsp::SignalQ15"){

    }
};

class SignalQ31Test : public SignalTest<SignalQ31> {
public:
    SignalQ31Test() : SignalTest("dsp::SignalQ31"){

    }
};

class SignalF32Test : public SignalTest<SignalF32> {
public:
    SignalF32Test() : SignalTest("dsp::SignalF32"){

    }

};


#endif // SIGNALTEST_HPP
