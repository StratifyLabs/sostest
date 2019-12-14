#include <stdint.h>
#include <stdio.h>

#include <sapi/sys.hpp>
#include <sapi/test.hpp>

#include "SocketTest.hpp"
#include "SecureSocketTest.hpp"
#include "HttpClientTest.hpp"
#include "ToolkitDatabaseObjectTest.hpp"

//update flags
enum {
	//Test flags occupy lower bit values
	SOCKET_TEST_FLAG = (1<<5),
	SECURE_SOCKET_TEST_FLAG = (1<<6),
	HTTP_CLIENT_TEST_FLAG = (1<<7),
	HTTP_SERVER_TEST_FLAG = (1<<8),
	TOOLKIT_DATABASE_OBJECT_TEST_FLAG = (1<<9)
};

u32 decode_cli(const Cli & cli);
void show_usage(const Cli & cli);

int main(int argc, char * argv[]){
	Cli cli(argc, argv);
	cli.set_publisher("Stratify Labs, Inc");
	cli.handle_version();
	u32 o_flags;

	o_flags = decode_cli(cli);

	if( o_flags == 0 ){
		show_usage(cli);
		exit(0);
	}

   Test::initialize(
            Test::Name(cli.name()),
            Test::Version(cli.version()),
            Test::GitHash(SOS_GIT_HASH)
            );

	if( o_flags & SOCKET_TEST_FLAG ){
		SocketTest test;
		test.execute(o_flags);
	}

	if( o_flags & SECURE_SOCKET_TEST_FLAG ){
		SecureSocketTest test;
		test.execute(o_flags);
	}

	if( o_flags & HTTP_CLIENT_TEST_FLAG ){
		HttpClientTest test;
		test.execute(o_flags);
	}

	if( o_flags & TOOLKIT_DATABASE_OBJECT_TEST_FLAG ){
		ToolkitDatabaseObjectTest test;
		String project = cli.get_option(
					"project",
					Cli::Description("specify the cloud project name")
					);
		test.set_project( project );
		test.execute(o_flags);
	}

	Test::finalize();
	return 0;

}

u32 decode_cli(const Cli & cli){
   u32 o_flags = 0;
   o_flags = Test::parse_execution_flags(cli);
   o_flags |= Test::parse_test(cli, "socket", SOCKET_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "secureSocket", SECURE_SOCKET_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "httpClient", HTTP_CLIENT_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "toolkitDatabaseObject", TOOLKIT_DATABASE_OBJECT_TEST_FLAG);
	return o_flags;
}

void show_usage(const Cli & cli){
	cli.show_options();
}



