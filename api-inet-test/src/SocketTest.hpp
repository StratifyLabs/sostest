#ifndef SOCKETTEST_HPP
#define SOCKETTEST_HPP
#include <sapi/inet.hpp>
#include <sapi/test.hpp>

class SocketTest : public Test {
public:
	SocketTest();

	bool execute_class_api_case();
	bool execute_class_stress_case();
	bool udp_server_listening;
private:
	bool execute_socket_option_case();
	bool execute_socket_address_info_case();
	bool execute_socket_address_case();
	bool execute_socket_case();
	bool execute_socket_case_udp();
	bool execute_socket_case_raw();
	int family = SocketAddressInfo::FAMILY_INET;//use common for thread use
	bool thread_running = true;
	static void * listen_on_localhost_thread_function(void * args){
		return ((SocketTest*)args)->listen_on_localhost();
	}
	static void * listen_on_localhost_thread_function_udp(void * args){
		return ((SocketTest*)args)->listen_on_localhost_udp();
	}
	static void * listen_on_localhost_thread_function_ping_pong(void * args){
		return ((SocketTest*)args)->listen_on_localhost_ping_pong();
	}
	static void * listen_on_localhost_thread_function_ping_pong_udp(void * args){
		return ((SocketTest*)args)->listen_on_localhost_ping_pong_udp();
	}

	void * listen_on_localhost();
	void * listen_on_localhost_udp();
	void * listen_on_localhost_ping_pong();
	void * listen_on_localhost_ping_pong_udp();
};

#endif // SOCKETTEST_HPP
