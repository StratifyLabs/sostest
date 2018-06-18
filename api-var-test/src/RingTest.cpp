#include <sapi/var.hpp>
#include "RingTest.hpp"

RingTest::RingTest(): Test("var::Ring"){

}

/*! \details test "performance" a var::Ring
 *
 * @return false if some test failed
 */
bool RingTest::execute_class_performance_case(){
    bool result = true;

    return result;
}
/*! \details test "stress" a var::Ring
 *
 * @return false if some test failed
 */
bool RingTest::execute_class_stress_case(){
    bool result = true;

    return result;
}
/*! \details test "api" a var::Ring
 *  constructors,write,read,size,is_overflow_allowed
 * set_overflow_allowed
 * @return false if some test failed
 */
bool RingTest::execute_class_api_case(){
    bool result = true;
    //posible value for parametrs

    //overflow allowed
    const u8 size_packet = 8;
    u8 packet_write[size_packet];
    u8 packet_read[size_packet];
    u8 buffer[size_packet*8];
    u32 writed_data_size;
    Ring<u8> ring1(buffer, sizeof(buffer));
    Ring<u8> ring2(size_packet*16);
    //fill write buffer for test
    for (u16 i=0; i<size_packet; i++){
        packet_write[i] = i;
    }
    //init control
    if (!ring1.is_overflow_allowed()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(!ring2.is_overflow_allowed()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if((ring1.data() == 0) ||
            (ring1.capacity() < sizeof(buffer)) ||
            (ring1.size() != sizeof(buffer)) ){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if((ring2.data() == 0) ||
            (ring2.capacity() < 128) ||
            (ring2.size() != 128)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //is_overflow_allowed
    //write and read
    ring1.write(packet_write, size_packet);
    ring1.read(packet_read, size_packet);
    if(memcmp(packet_write, packet_read, size_packet)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    for (u16 i=0; i < ring1.capacity()/size_packet;i++){
        writed_data_size = ring1.write(packet_write, size_packet);
        if(writed_data_size != size_packet){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
    }
    //repeat for overflow check
    for (u16 i=0; i < ring1.capacity()/size_packet;i++){
        writed_data_size = ring1.write(packet_write, size_packet);
        if(writed_data_size != size_packet){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
    }
    //flush, used and free size
    ring1.flush();
    ring2.flush();
    if (ring1.calc_used()!=0 || ring2.calc_used()!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(ring1.calc_free()!=ring1.size() || ring2.calc_free()!=ring2.size()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //write
    for (u16 i=0; i < ring1.size()/size_packet;i++){
        ring1.write(packet_write, size_packet);
        if(ring1.calc_used() != (u32)(size_packet + i*size_packet) ||
                ring1.calc_free() != (ring1.size() - (size_packet + i*size_packet))){
            //print_case_message("calc_used - %llu, calc_free - %llu, calc used %llu",ring1.calc_used(),ring1.calc_free(),(u32)(size_packet + i*size_packet));
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
    }
    for (u16 i=0; i < ring2.size()/size_packet;i++){
        ring2.write(packet_write, size_packet);
        if(ring2.calc_used() != (u32)(size_packet + i*size_packet) ||
                ring2.calc_free() != (ring2.size() - (size_packet + i*size_packet))){
            //print_case_message("calc_used - %llu, calc_free - %llu, calc used %llu",ring2.calc_used(),ring2.calc_free(),(u32)(size_packet + i*size_packet));
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
    }
    if(ring1.calc_free()!=0 || ring1.calc_size() != ring1.size()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(ring1.calc_free()!=0 || ring1.calc_size() != ring1.size()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //read
    for (u16 i=0; i < ring1.size()/size_packet;i++){
        memset(packet_read, 0, size_packet);
        ring1.read(packet_read, size_packet);
        if(memcmp(packet_write, packet_read, size_packet)){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
    }
    for (u16 i=0; i < ring2.size()/size_packet;i++){
        memset(packet_read, 0, size_packet);
        ring2.read(packet_read, size_packet);
        if(memcmp(packet_write, packet_read, size_packet)){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
    }

    //overflow not allowed
    ring1.flush();
    ring2.flush();
    ring1.set_overflow_allowed(false);
    ring2.set_overflow_allowed(false);
    if(ring1.is_overflow_allowed()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(ring2.is_overflow_allowed()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    ring1.write(packet_write, size_packet);
    ring1.read(packet_read, size_packet);
    if(memcmp(packet_write, packet_read, size_packet)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    for (u16 i=0; i < ring1.size()/size_packet;i++){
        writed_data_size = ring1.write(packet_write, size_packet);
        if(writed_data_size != size_packet){
            print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
            result = false;
        }
    }
    //repeat for overflow check
    for (u16 i=0; i < ring1.size()/size_packet;i++){
        writed_data_size = ring1.write(packet_write, size_packet);
        if(writed_data_size != 0){
            //print_case_message("writed_data_size %llu", writed_data_size);
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
    }
    ring1.flush();
    ring2.flush();
    //write
    for (u16 i=0; i < ring1.size()/size_packet;i++){
        ring1.write(packet_write, size_packet);
        if(ring1.calc_used() != (u32)(size_packet + i*size_packet) ||
                ring1.calc_free() != (ring1.size() - (size_packet + i*size_packet))){
            //print_case_message("calc_used - %llu, calc_free - %llu");
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
    }
    for (u16 i=0; i < ring2.size()/size_packet;i++){
        ring2.write(packet_write, size_packet);
        if(ring2.calc_used() != (u32)(size_packet + i*size_packet) ||
                ring2.calc_free() != (ring2.size() - (size_packet + i*size_packet))){
            //print_case_message("calc_used - %llu, calc_free - %llu");
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
    }
    if(ring1.calc_free()!=0 || ring1.calc_size() != ring1.size()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(ring1.calc_free()!=0 || ring1.calc_size() != ring1.size()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    //read
    for (u16 i=0; i < ring1.size()/size_packet;i++){
        memset(packet_read, 0, size_packet);
        ring1.read(packet_read, size_packet);
        if(memcmp(packet_write, packet_read, size_packet)){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
    }
    for (u16 i=0; i < ring2.size()/size_packet;i++){
        memset(packet_read, 0, size_packet);
        ring2.read(packet_read, size_packet);
        if(memcmp(packet_write, packet_read, size_packet)){
            print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
            result = false;
            break;
        }
    }
    //imposible value
    ring1.flush();
    ring2.flush();
    const u16 size_packet_overflow = 256;
    u8 packet_write_overflow[size_packet_overflow];
    u8 packet_read_overflow[size_packet_overflow];
    //fill write buffer for test
    for (u16 i=0; i<size_packet_overflow; i++){
        packet_write_overflow[i] = i;
    }
    writed_data_size = ring1.write(packet_write_overflow,size_packet_overflow);
    if(writed_data_size != ring1.calc_size()){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    writed_data_size = ring1.read(packet_read_overflow,size_packet_overflow);
    if(writed_data_size != ring1.calc_size()){
        print_case_message("read data size %lu", writed_data_size, ring1.calc_size());
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(memcmp(packet_read_overflow, packet_write_overflow, ring1.calc_size())){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    return result;
}

