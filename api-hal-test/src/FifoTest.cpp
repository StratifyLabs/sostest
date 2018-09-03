#include "FifoTest.hpp"
#include <sapi/var.hpp>
FifoTest::FifoTest() : Test("hal::Fifo"){

}

/*@brief api test for hal/Fifo use "api-hal-test -fifo -api"
 * FifoInfo used for get info from open file?
 *
 *
 * Fifo
 *
 */
bool FifoTest::execute_class_api_case(){
    bool result = true;
    fifo_info_t fifo_info_s;
    fifo_info_s.size = 256;
    fifo_info_s.overflow = 0;
    FifoInfo fifo_info;
    FifoInfo fifo_info_init(fifo_info_s);
    //fifo info start
    if(fifo_info.is_valid()){
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        result = false;
    }
    if(fifo_info.size()){
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        result = false;
    }
    if(!fifo_info_init.is_valid()){
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        result = false;
    }
    if(!fifo_info_init.size()){
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        result = false;
    }
    if(fifo_info_init.is_overflow()){
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        result = false;
    }

    //fifo info end
    Fifo fifo;

    if( fifo.open("/dev/fifo", Fifo::RDWR) < 0 ){
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        result = false;
    }
    //fifo.init(); !!!
    fifo_info = fifo.get_info();
    //write some data to the fifo
    char write_buff[] = "Testing fifo write";
    char read_buff[sizeof(write_buff)];
    fifo.write(write_buff,sizeof (write_buff));
    fifo_info = fifo.get_info();
    fifo.get_info(fifo_info);
/*    print_case_message("size %d,used %d,overflow %d,valid %d",fifo_info.size(),fifo_info.used(),fifo_info.is_overflow(),\
                       fifo_info.is_valid());*/

    fifo.read(read_buff,sizeof (write_buff));
    if(memcmp(write_buff,read_buff,sizeof(write_buff))){
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        result = false;
    }
    fifo.write(write_buff);
    fifo.flush();
    fifo.read(read_buff,sizeof(write_buff));
    if(!memcmp(write_buff,read_buff,sizeof(write_buff))){
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        print_case_message("%s",read_buff);
        result = false;
    }

    fifo.close();
    return result;
}

bool FifoTest::execute_class_performance_case(){
    bool result = true;

    return result;
}

bool FifoTest::execute_class_stress_case(){
    bool result = true;
    Fifo fifo;

    if( fifo.open("/dev/fifo", Fifo::RDWR) < 0 ){
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        result = false;
    }
    //fifo.init(); !!!
    //write some data to the fifo
    String messege("messege");
    String recv;
    messege.append("1");
    fifo.write(messege.c_str(),messege.size());
    messege.erase(messege.size()-2,1);
    messege.append("2");
    print_case_message("mess %s",messege.c_str());
    fifo.write(messege.c_str(),messege.size());
    char read_buff[messege.size()];
    u16 len = fifo.read(read_buff,messege.size());
    print_case_message("recv %s %d",read_buff,len);
    if(memcmp(messege.c_str(),read_buff,messege.size())){
        print_case_message("Failed %s %d:", __FILE__, __LINE__);
        result = false;
    }
    fifo.flush();
    fifo.close();

    return result;
}
