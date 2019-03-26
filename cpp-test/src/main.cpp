
#include <cstdio>
#include <sapi/sys.hpp>
#include "StringTest.hpp"
#include "ContainerTest.hpp"

bool handle_tests(const Cli & cli);

int main(int argc, char * argv[]){
	Cli cli(argc, argv, SOS_GIT_HASH);

	if( handle_tests(cli) ){
		exit(0);
	}

	return 0;
}


bool handle_tests(const Cli & cli){


	if( cli.get_option("test") == "true" ){

		u32 o_execute_flags = 0;

		if( cli.get_option("api") == "true" ){ o_execute_flags |= Test::EXECUTE_API; }
		if( cli.get_option("stress") == "true" ){ o_execute_flags |= Test::EXECUTE_STRESS; }
		if( cli.get_option("performance") == "true" ){ o_execute_flags |= Test::EXECUTE_PERFORMANCE; }

		if( o_execute_flags == 0 ){
			o_execute_flags = Test::EXECUTE_ALL;
		}

		Test::initialize(cli.name(), cli.version(), SOS_GIT_HASH);

		if( cli.get_option("string") == "true" ){
			StringTest test;
			test.execute(o_execute_flags);
		}


		if( cli.get_option("container") == "true" ){
			ContainerTest test;
			test.execute(o_execute_flags);
		}

		Test::finalize();

		return true;
	}

	return false;

}
