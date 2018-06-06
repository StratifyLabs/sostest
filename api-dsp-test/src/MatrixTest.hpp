#ifndef MATRIX_TEST_HPP
#define MATRIX_TEST_HPP

#include <sapi/test.hpp>
#include <sapi/sys.hpp>
#include <sapi/dsp.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>

template<typename SignalType> class MatrixTest : public Test {
public:
    MatrixTest(const char * name) : Test(name){}



    bool execute_class_api_case(){
        bool result = true;
        Timer t;

        api::DspWorkObject::request_arm_dsp_api();

        //change to FIFO style scheduling for more accurate performance comparisons
        Sched::set_scheduler(Sched::get_pid(), Sched::FIFO, 30);

        //restore the scheduling style
        Sched::set_scheduler(Sched::get_pid(), Sched::OTHER, 0);

        return result;
    }

    bool execute_class_performance_case(){

        return true;
    }


    bool execute_class_stress_case(){
        return true;
    }

};

class MatrixQ15Test : public MatrixTest<SignalQ15> {
public:
    MatrixQ15Test() : MatrixTest("dsp::MatrixQ15"){}
};

class MatrixQ31Test : public MatrixTest<SignalQ31> {
public:
    MatrixQ31Test() : MatrixTest("dsp::MatrixQ31"){

    }
};

class MatrixF32Test : public MatrixTest<SignalF32> {
public:
    MatrixF32Test() : MatrixTest("dsp::MatrixF32"){

    }
};

#endif // SIGNALTEST_HPP
