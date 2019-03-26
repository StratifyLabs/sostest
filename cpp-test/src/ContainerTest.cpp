#include "ContainerTest.hpp"



#include <array>
#include <bitset>
#include <deque>
#include <forward_list>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <string>

using namespace std;

ContainerTest::ContainerTest() : Test("ContainerTest"){

}


bool ContainerTest::execute_class_api_case(){


	open_case("array");
	execute_array_api_case();
	close_case();

	open_case("bitset");
	execute_bitset_api_case();
	close_case();

	open_case("deque");
	execute_deque_api_case();
	close_case();

	open_case("forward_list");
	execute_forward_list_api_case();
	close_case();

	open_case("list");
	execute_list_api_case();
	close_case();

	open_case("map");
	execute_map_api_case();
	close_case();

	open_case("queue");
	execute_queue_api_case();
	close_case();

	open_case("set");
	execute_set_api_case();
	close_case();

	open_case("stack");
	execute_stack_api_case();
	close_case();

	open_case("unordered_map");
	execute_unordered_map_api_case();
	close_case();

	open_case("unordered_set");
	execute_unordered_set_api_case();
	close_case();

	open_case("vector");
	execute_vector_api_case();
	close_case();

	return case_result();
}


bool ContainerTest::execute_array_api_case(){
	return case_result();
}

bool ContainerTest::execute_bitset_api_case(){
		return case_result();
	}

bool ContainerTest::execute_deque_api_case(){
	return case_result();
}

bool ContainerTest::execute_forward_list_api_case(){
	return case_result();
}

bool ContainerTest::execute_list_api_case(){
	return case_result();
}

bool ContainerTest::execute_map_api_case(){
	return case_result();
}

bool ContainerTest::execute_queue_api_case(){
	return case_result();
}
bool ContainerTest::execute_set_api_case(){
	return case_result();
}
bool ContainerTest::execute_stack_api_case(){
	return case_result();
}
bool ContainerTest::execute_unordered_map_api_case(){
	return case_result();
}
bool ContainerTest::execute_unordered_set_api_case(){
	return case_result();
}
bool ContainerTest::execute_vector_api_case(){

	vector<u32> vector_u32;

	const u32 max = 200;

	for(u32 i=0; i < max; i++){
		vector_u32.push_back(i);
	}

	if( vector_u32.at(9) != 9 ){
		print_case_failed("failed %s():%d", __FUNCTION__, __LINE__);
	}

	if( vector_u32.size() != max ){
		print_case_failed("failed %s():%d", __FUNCTION__, __LINE__);
	}

	while( vector_u32.size() > 0 ){
		vector_u32.pop_back();
	}


	return case_result();
}
