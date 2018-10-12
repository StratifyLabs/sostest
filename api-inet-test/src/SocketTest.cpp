#include <sapi/inet.hpp>
#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/chrono.hpp>

#include "SocketTest.hpp"

SocketTest::SocketTest() : Test("SocketTest"){}


bool SocketTest::execute_class_api_case(){

	if( !execute_socket_address_info_case() ){
		print_case_failed("socket address info case failed");
		return case_result();
	}

	if( !execute_socket_case() ){
		print_case_failed("socket case failed");
		return case_result();
	}

	return case_result();
}

bool SocketTest::execute_socket_case(){

	//start the listener
	Thread thread;

	if( thread.create(listen_on_localhost_thread_function, this) < 0 ){
		print_case_failed("Failed to create listener thread");
		return case_result();
	}

	Timer::wait_milliseconds(1000);

	Vector<SocketAddressInfo> list = SocketAddressInfo().fetch_node("localhost");

	if( list.count() == 0 ){
		print_case_failed("Failed to fetch node");
		return case_result();
	}

	Socket local_host_socket;
	//SocketAddress localhost_address(SocketAddressIpv4(0, 8080));
	SocketAddress localhost_address(list.at(0), 8080);

	if( local_host_socket.create(localhost_address) < 0){
		print_case_failed("Failed to create client socket");
		return case_result();
	}

	if( local_host_socket.connect(localhost_address) < 0 ){
		print_case_failed("Failed to connect to socket");
		return case_result();
	}

	String test("Testing");
	Data reply(256);

	if( local_host_socket.write(test) < 0 ){
		print_case_failed("Failed to write client socket (%d, %d)", test.size(), local_host_socket.error_number());
		perror("failed to write");
		return case_result();
	}

	reply.fill(0);
	if( local_host_socket.read(reply) < 0 ){
		print_case_failed("Failed to read client socket");
		return case_result();
	}

	print_case_message("read '%s' from socket", reply.to_char());

	if( test != reply.to_char() ){
		print_case_failed("did not get an echo on localhost");
	}

	local_host_socket.close();

	return case_result();
}

void * SocketTest::listen_on_localhost(){
	Vector<SocketAddressInfo> list = SocketAddressInfo().fetch_node("localhost");

	if( list.count() == 0 ){
		print_case_failed("Failed to fetch node");
		return 0;
	}

	Socket local_host_listen_socket;
	SocketAddress localhost_address(list.at(0), 8080);

	print_case_message("create socket at %s", localhost_address.address_to_string().str());
	if( local_host_listen_socket.create(localhost_address) < 0 ){
		print_case_failed("failed to create socket");
		return 0;
	}

	//listen on port 80 of localhost
	local_host_listen_socket.clear_error_number();
	local_host_listen_socket << SocketOption().reuse_address() << SocketOption().reuse_port();

	if( local_host_listen_socket.error_number() != 0 ){
		print_case_failed("failed to set socket options (%d)", local_host_listen_socket.error_number());
	}

	//connect to port 80 of localhost
	if( local_host_listen_socket.bind_and_listen(localhost_address) < 0 ){
		print_case_failed("Failed to bind to localhost (%d)", local_host_listen_socket.error_number());
		return 0;
	}


	print_case_message("Listening on localhost:%d", localhost_address.port());
	//now accept -- this will block until a request arrives
	SocketAddress accepted_address;
	Socket local_host_session_socket = local_host_listen_socket.accept(accepted_address);

	Data incoming(256);
	local_host_session_socket.read(incoming);
	local_host_session_socket.write(incoming);

	local_host_session_socket.close();

	if( local_host_listen_socket.close() < 0 ){
		print_case_failed("failed to close socket");
		return 0;
	}

	return 0;
}



bool SocketTest::execute_socket_address_info_case(){

	SocketAddressInfo address_info;

	print_case_message("get list from stratifylabs.co");
	Vector<SocketAddressInfo> list = address_info.fetch_node("stratifylabs.co");

	print_case_message("got %d entries", list.count());
	for(u32 i=0; i < list.count(); i++){
		open_case(String().format("entry-%d", i));
		SocketAddress address(list.at(i), 80);
		if( address.family() == SocketAddressInfo::FAMILY_INET ){
			print_case_message("family is ipv4");
		} else {
			print_case_message("family is other:%d", address.family());
		}

		if( address.type() == SocketAddressInfo::TYPE_STREAM ){
			print_case_message("type is stream");
		} else {
			print_case_message("type is other:%d", address.type());
		}

		if( address.protocol() == SocketAddressInfo::PROTOCOL_TCP ){
			print_case_message("protocol is tcp");
		} else {
			print_case_message("protocol is other:%d", address.protocol());
		}

		print_case_message("address: %s", address.address_to_string().str());
		print_case_message("port is %d", address.port());
		close_case();
	}

	return true;
}
