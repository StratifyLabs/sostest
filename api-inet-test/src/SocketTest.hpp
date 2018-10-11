#ifndef SOCKETTEST_HPP
#define SOCKETTEST_HPP

#include <sapi/test.hpp>

class SocketTest : public Test {
public:
	SocketTest();

	bool execute_class_api_case();

private:
	bool execute_socket_address_info_case();
	bool execute_socket_case();
};

#endif // SOCKETTEST_HPP
