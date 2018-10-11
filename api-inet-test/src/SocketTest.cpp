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


	Vector<SocketAddressInfo> list = SocketAddressInfo().fetch_node("stratifylabs.co");

	if( list.count() == 0 ){
		print_case_failed("Failed to fetch node");
		return case_result();
	}

	Socket socket;
	SocketAddress address(list.at(0), 80);


	print_case_message("create socket at %s", address.address_to_string().str());
	if( socket.create(address) < 0 ){
		print_case_failed("failed to create socket");
		return case_result();
	}


	if( socket.close() < 0 ){
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
