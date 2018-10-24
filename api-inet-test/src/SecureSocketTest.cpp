#include <sapi/inet.hpp>
#include <sapi/var.hpp>

#include "SecureSocketTest.hpp"

SecureSocketTest::SecureSocketTest() : Test("SecureSocketTest"){

}


bool SecureSocketTest::execute_class_api_case(){
	SocketAddressInfo address_info;
	Vector<SocketAddressInfo> addresses;

	addresses = address_info.fetch_node("stratify-e6020.firebaseio.com");
	//addresses = address_info.fetch_node("stratifylabs.co");

	if( addresses.count() > 0 ){
		SocketAddress address(addresses.at(0), 80);
		print_case_message("Cannon name is %s", addresses.at(0).canon_name().str());
		print_case_message("connect to %s", address.address_to_string().str());

		SecureSocket socket;

		print_case_message("Get data from secure server");
		HttpClient http_client(socket);
		http_client.get("https://stratify-e6020.firebaseio.com/cloud/project/-KYds7ufk3y4jZe2lnKS/changes.json");

		//read the response
		open_case("response");
		printf("%s", http_client.response().str());
		close_case();

		print_case_message("close secure socket");
		//socket.close();

	} else {
		print_case_failed("failed to retrieve address info for stratify-e6020.firebaseio.com");
	}


	return case_result();
}
