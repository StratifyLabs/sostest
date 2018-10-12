#include <sapi/inet.hpp>
#include <sapi/var.hpp>
#include "SocketTest.hpp"

SocketTest::SocketTest() : Test("SocketTest"){

}


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

	Vector<SocketAddressInfo> list = SocketAddressInfo().fetch_node("localhost");

	if( list.count() == 0 ){
		print_case_failed("Failed to fetch node");
		return case_result();
	}

	Socket local_host_listen_socket;
	SocketAddress localhost_address(list.at(0), 8080);

	print_case_message("create socket at %s", localhost_address.address_to_string().str());
	if( local_host_listen_socket.create(localhost_address) < 0 ){
		print_case_failed("failed to create socket");
		return case_result();
	}

	//listen on port 80 of localhost
	local_host_listen_socket.clear_error_number();
	local_host_listen_socket << SocketOption().reuse_address() << SocketOption().reuse_port();

	if( local_host_listen_socket.error_number() != 0 ){
		print_case_failed("failed to set socket options (%d)", local_host_listen_socket.error_number());
	}

	//connect to port 80 of localhost
	perror("Pre bind-error");
	if( local_host_listen_socket.bind(localhost_address) < 0 ){
		print_case_failed("Failed to bind to localhost (%d)", local_host_listen_socket.error_number());
		perror("failed to bind");
		return case_result();
	}


	if( local_host_listen_socket.listen() < 0 ){
		print_case_failed("Failed to list on localhost");
		return case_result();
	}

	print_case_message("Listening on localhost:%d", local_host_listen_socket.address().port());
	//now accept -- this will block until a request arrives
	Socket local_host_session_socket = local_host_listen_socket.accept();

	Data incoming(1024);
	int bytes_received = local_host_session_socket.read(incoming);

	String reply;
	reply = "Hello\n";
	if( bytes_received > 0 ){
		local_host_session_socket.write(reply);
	}

	if( local_host_listen_socket.close() < 0 ){
		print_case_failed("failed to close socket");
		return case_result();
	}


	return case_result();
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
