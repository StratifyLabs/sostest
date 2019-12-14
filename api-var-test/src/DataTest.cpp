

#include "DataTest.hpp"
int DataTest::recursive_number = 0;

DataTest::DataTest() : Test("var::Data"){
	recursive_number = 0;
	Data::reclaim_heap_space();
}


bool DataTest::execute_class_api_case(){
	bool result = true;

	if( execute_fill() == false ){
		print_case_message("why:0", "execute_fill()");
		result = false;
	}

	if( execute_alloc() == false ){
		print_case_message("why:0", "execute_alloc()");
		result = false;
	}

	return result;
}

bool DataTest::execute_fill(){
	bool result = true;
	Data data(128);
	char buffer[128];

	memset(buffer, 0, 128);
	data.fill(0);
	if( memcmp(data.to_const_void(), buffer, 128) != 0 ){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	data.fill(2);
	if( memcmp(data.to_const_void(), buffer, 128) == 0 ){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	data.clear();
	if( memcmp(data.to_const_void(), buffer, 128) != 0 ){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}

	data.fill(0xaa);
	memset(buffer, 0xaa, 128);

	if( memcmp(data.to_const_void(), buffer, 128) != 0 ){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}

	return result;
}


bool DataTest::execute_alloc(){
	bool result = true;
	u16 data_size = 512;
	//test every API to see if it works as expected -- included giving it invalid values
	Data data;
	Data dynamic_data(data_size);

	if(data.data() != 0){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}

	if(data.to_const_void() == 0){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if( dynamic_data.data() == 0 ){
		//failed to allocate memory
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if( dynamic_data.to_const_void() == 0 || (dynamic_data.capacity() < data_size)){
		//failed to allocate memory
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	data_size-=Data::minimum_capacity();
	dynamic_data.allocate(data_size, Data::IsResize(true) );
	if( dynamic_data.to_const_void() == 0 ||
		 dynamic_data.data() == 0 ||
		 (dynamic_data.capacity() < data_size)){
		//failed to allocate memory
		print_case_message("Failed %s:%d %d < %d", __PRETTY_FUNCTION__, __LINE__, dynamic_data.capacity(), data_size);
		result = false;
	}
	data_size-=Data::minimum_capacity();
	dynamic_data.allocate(data_size);
	if( dynamic_data.to_const_void() == 0 ||
		 dynamic_data.data() == 0 ||
		 (dynamic_data.capacity() < data_size)){
		//failed to allocate memory
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	data_size-=Data::minimum_capacity();
	dynamic_data.resize(data_size);
	if( dynamic_data.to_const_void() == 0 ||
		 dynamic_data.data() == 0 ||
		 (dynamic_data.capacity() < data_size)){
		//failed to allocate memory
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}


	return result;
}

bool DataTest::execute_class_performance_case(){
	bool result = true;
	int i;
	for(i=0; i < 5000; i++){
		u32 data_size = (rand() & 0xfff) + 1; //12 bits is up to 4096
		Data data(data_size);
		if( data.data() == 0 ){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
			break;
		}

		char buffer[data_size];
		memset(buffer, 0xaa, data_size);
		data.fill(0xaa);
		if( memcmp(buffer, data.to_const_void(), data_size) ){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
			break;
		}
		memset(buffer, 0x00, data_size);
		//add clear test
		data.clear();
		if( memcmp(buffer, data.to_const_void(), data_size) ){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
			break;
		}
		if (data_size){
			char* t;
			t = data.to_char();
			//change one byte in data
			t[data_size-1] = 0x0e;
			if( !memcmp(buffer, data.to_const_void(), data_size) ){
				print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
				result = false;
				break;
			}
		}
	}

	return result;
}



bool DataTest::execute_class_stress_case(){
	bool result = true;
	u32 i;

	for(i=0; i < 1000; i++){
		//add recursive test
		u16 size_temp = rand() % 256;
		Data data(size_temp);
		recursive_number = 0;
		if(!execute_recursive(data)){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
		}
#if !defined __link
		//this is a special case on StratifyOS that causes malloc to release memory back to the stack
		::free((void*)1);
#endif
	}

	return result;
}
/*! \details test used for "stress" test in var::String
	* @return false if some test failed true if passed
 */

bool DataTest::execute_recursive(Data data){
	recursive_number++;
	if (data.capacity() > Data::minimum_capacity()*2){
		Data data_new(data.capacity()-Data::minimum_capacity()*2);
		if( data_new.data() == 0 ){
			//failed to allocate memory
			print_case_message("Failed %s:%d %d %d", __PRETTY_FUNCTION__, __LINE__, data.capacity()-Data::minimum_capacity(), recursive_number);
			return false;
		} else {
			char fill_temp;
			//remember value
			fill_temp = recursive_number;
			data_new.fill(fill_temp);
			if (!execute_recursive(data_new)){
				//print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
				return false;
			}
			char buffer[data.capacity()];
			//recursive value changes after execute_recursive
			memset(buffer, fill_temp, data_new.capacity());
			if( memcmp(buffer, data_new.to_const_void(), data_new.capacity()) ){
				print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
				return false;
			}
		}
	}
	return true;
}
