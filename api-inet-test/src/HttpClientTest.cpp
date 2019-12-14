#include "HttpClientTest.hpp"
#include <sapi/sys.hpp>
#include <sapi/fs.hpp>
#include <sapi/var.hpp>

HttpClientTest::HttpClientTest() : Test("HttpClientTest"){

}


bool HttpClientTest::execute_class_api_case(){
	SecureSocket socket;
	HttpClient client(socket);
	DataFile response(OpenFlags::append());

	print_case_message("download from secure source");
	if( client.get(
			 Http::UrlEncodedString("https://stratifylabs.co/files/config.json"),
			 Http::ResponseFile(response)
			 ) < 0 ){
		print_case_failed("failed to download config file");
	} else {
		print_case_message("downloaded config to RAM");
	}

	print_case_message("done");
	return case_result();
}
