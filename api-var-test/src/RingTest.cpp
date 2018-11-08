#include <sapi/var.hpp>
#include "RingTest.hpp"

RingTest::RingTest(): Test("var::Ring"){
	Data::reclaim_heap_space();
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

	//overflow not allowed
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

	return result;
}

