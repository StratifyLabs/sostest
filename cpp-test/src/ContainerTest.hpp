#ifndef CONTAINERTEST_HPP
#define CONTAINERTEST_HPP

#include <sapi/test.hpp>

class ContainerTest : public Test {
public:
	ContainerTest();
	bool execute_class_api_case();

private:
	bool execute_array_api_case();
	bool execute_bitset_api_case();
	bool execute_deque_api_case();
	bool execute_forward_list_api_case();
	bool execute_list_api_case();
	bool execute_map_api_case();
	bool execute_queue_api_case();
	bool execute_set_api_case();
	bool execute_stack_api_case();
	bool execute_unordered_map_api_case();
	bool execute_unordered_set_api_case();
	bool execute_vector_api_case();

};

#endif // CONTAINERTEST_HPP
