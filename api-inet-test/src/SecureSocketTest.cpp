#include <sapi/inet.hpp>
#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/fs.hpp>

#include "SecureSocketTest.hpp"

SecureSocketTest::SecureSocketTest() : Test("SecureSocketTest"){

}


bool SecureSocketTest::execute_class_api_case(){
	SocketAddressInfo address_info;
	Vector<SocketAddressInfo> addresses;
	int result;

	print_case_message("fetch node for firebaseio");
	addresses = address_info.fetch_node("stratify-e6020.firebaseio.com");
	//addresses = address_info.fetch_node("stratifylabs.co");

	if( addresses.count() > 0 ){
		SocketAddress address(addresses.at(0), 80);
		print_case_message("Cannon name is %s", addresses.at(0).canon_name().cstring());
		print_case_message("connect to %s", address.address_to_string().cstring());
		SecureSocket socket;

		print_case_message("Get data from secure server");
		HttpClient http_client(socket);

		http_client.set_keep_alive(true);

		for(u32 i=0; i < 2; i++){
			fs::DataFile response_file(fs::OpenFlags::append_write_only());
			if( (result = http_client.get(
					  "https://stratify-e6020.firebaseio.com/cloud/project/-KYds7ufk3y4jZe2lnKS/changes.json",
					  Http::ResponseFile(response_file)
					  )
				  ) < 0 ){
				print_case_failed("failed to get data (%d, %d)", result, http_client.error_number());
				print_case_message("header:%s", http_client.header().cstring());
				return case_result();
			} else {

				//read the response
				open_case("response");
				String s(response_file.data());
				print_case_message("%s", s.cstring());
				close_case();

				print_case_message("pairs %ld", http_client.header_response_pairs().count());
			}
		}

		http_client.close_connection();

	} else {
		print_case_failed("failed to retrieve address info for stratify-e6020.firebaseio.com");
	}


	return case_result();
}
