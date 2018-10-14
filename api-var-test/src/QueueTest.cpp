#include <sapi/var.hpp>
#include "QueueTest.hpp"

QueueTest::QueueTest() : Test("var::Queue"){
#if !defined __link
	::free((void*)1);
#endif
}
/*! \details performance test for var::Queue
 * @return false if same test failed
 */
bool QueueTest::execute_class_performance_case(){
	bool result = true;
	Queue <u8>u8_queue;
	Queue <u32>u32_queue;
	Queue <double>double_queue;
	for (u32 j =0;j<40;j++){
		for (u32 i = 0;i<500;i++){
			u8_queue.push(i);
			u32_queue.push(i);
			double_queue.push((double)i);
		}
		for (u32 i = 0;i<500;i++){
			if((u8_queue.front()!=(u8)i)||(u32_queue.front()!=i)||(double_queue.front()!=(double)i)){
				print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
				result = false;
				break;
			}
			u8_queue.pop();
			u32_queue.pop();
			double_queue.pop();
		}
		if(!u8_queue.is_empty() || !u32_queue.is_empty() || !double_queue.is_empty()){
			u8_queue.clear();
			u32_queue.clear();
			double_queue.clear();
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
		}
	}
	return result;
}
/*! \details stress test for api a var::Queue
 * @return false if same test failed
 */

bool QueueTest::execute_class_stress_case(){
	bool result = true;
	const u32 inner_max = 200;
	const u32 outer = 1000;
	Queue <u8>u8_queue;
	Queue <u32>u32_queue;
	Queue <double>double_queue;

	for (u32 j =0;j<outer;j++){
		u32 inner = rand() % inner_max + 1;

		for (u32 i = 0;i<inner;i++){
			u8_queue.push(i);
			u32_queue.push(i);
			double_queue.push((double)i);
		}
		for (u32 i = 0;i<inner;i++){
			if((u8_queue.front()!=(u8)i)||(u32_queue.front()!=i)||(double_queue.front()!=(double)i)){
				print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
				result = false;
				break;
			}
			u8_queue.pop();
			u32_queue.pop();
			double_queue.pop();
		}
		if(!u8_queue.is_empty() || !u32_queue.is_empty() || !double_queue.is_empty()){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			u8_queue.clear();
			u32_queue.clear();
			double_queue.clear();
			result = false;
		}
	}
	return result;
}
/*! \details api test for var::Queue
 * push,pop,clear,count,is_empty
 * @return false if same test failed
 */

bool QueueTest::execute_class_api_case(){
	bool result = true;
	Queue <u8>u8_queue;
	Queue <u32>u32_queue;
	Queue <float>float_queue;
	u32 i;
	u8_queue.push(1);
	u32_queue.push(0xffffffff);
	float_queue.push(3.1456f);
	if((u8_queue.front()!= 1)
			|| (u32_queue.front()!= 0xffffffff)
			|| (float_queue.front()!= 3.1456f)
			){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	u8_queue.pop();
	u32_queue.pop();
	float_queue.pop();
	if(!u8_queue.is_empty() || !u32_queue.is_empty() ||
			!float_queue.is_empty() ){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	for (i=0;i<250;i++){
		u8_queue.push(i);
		u32_queue.push(i);
		float_queue.push(float(i));
	}

	if((u8_queue.count()!= 250)
			|| (u32_queue.count()!= 250)
			||(float_queue.count() != 250)
			){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	for (i=0;i<249;i++){
		if ((u8_queue.back()!= 249)
			 || (u32_queue.back()!= 249)
			 || (float_queue.back()!= (float)249)
			 ){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
			break;
		}
		if ((u8_queue.front()!= i) || (u32_queue.front()!= i) ||
			 (float_queue.front()!= (float)i)){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
			break;
		}
		u8_queue.pop();
		u32_queue.pop();
		float_queue.pop();
	}
	if(i!=249){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}

	if((u8_queue.count()!= 1) || (u32_queue.count()!= 1) ||
			(float_queue.count() != 1)){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	u8_queue.clear();
	u32_queue.clear();
	float_queue.clear();
	if((u8_queue.count()!= 0) || (u32_queue.count()!= 0) ||
			(float_queue.count() != 0)){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}


	return result;
}
