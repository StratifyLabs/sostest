#ifndef SOCKETTEST_HPP
#define SOCKETTEST_HPP

#include <sapi/test.hpp>

class SocketTest : public Test {
public:
	SocketTest();

	bool execute_class_api_case();
    bool udp_server_listening;
private:
    bool execute_socket_property_case();
	bool execute_socket_address_info_case();
	bool execute_socket_case();

	static void * listen_on_localhost_thread_function(void * args){
		return ((SocketTest*)args)->listen_on_localhost();
	}

	void * listen_on_localhost();

};

#endif // SOCKETTEST_HPP
