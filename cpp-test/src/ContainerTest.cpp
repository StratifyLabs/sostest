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

	{
		Case cg(this, "array");
		execute_array_api_case();
	}

	{
		Case cg(this, "bitset");
		execute_bitset_api_case();
	}

	{
		Case cg(this, "deque");
		execute_deque_api_case();
	}

	{
		Case cg(this, "forward_list");
		execute_forward_list_api_case();
	}


	{
		Case cg(this, "list");
		execute_list_api_case();
	}

	{
		Case cg(this, "map");
		execute_map_api_case();
	}

	{
		Case cg(this, "queue");
		execute_queue_api_case();
	}

	{
		Case cg(this, "set");
		execute_queue_api_case();
	}

	{
		Case cg(this, "stack");
		execute_stack_api_case();
	}

	{
		Case cg(this, "unordered_map");
		execute_unordered_map_api_case();
	}

	{
		Case cg(this, "unordered_set");
		execute_unordered_set_api_case();
	}
	{
		Case cg(this, "vector");
		execute_vector_api_case();
	}

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
