
#include <cstdio>

#include "dhry.h"

#include <sapi/test.hpp>
#include <sapi/sys.hpp>
#include <sapi/var.hpp>

static int (*dmain)() = 0;


class Dhrystone : public Test {
public:

	Dhrystone(const ConstString & name) : Test(name){}


	bool execute_class_performance_case(){

		//1000 seconds is the baseline -- divide this by execution time to get score -- smaller time is a higher score
		const u32 baseline_microseconds = 1000000000UL;

		print_case_message_with_key("runs", String().format("%ld", NUMBER_OF_RUNS).cstring());
		if( dhry_main() < 0 ){
			return false;
		}
		u32 dmips = NUMBER_OF_RUNS*1000 / (case_timer().milliseconds() * 1757);
		print_case_message_with_key("score", String().format("%ld", baseline_microseconds / case_timer().microseconds() ).cstring());
		print_case_message_with_key("dmips", String().format("%ld", dmips).cstring());
		return true;
	}
};

int main(int argc, char * argv[]){
	Cli cli(argc, argv);

	dmain = (int (*)())kernel_request_api(0);

	Test::initialize("dhrystone", cli.version(), SOS_GIT_HASH);

	{ //these braces force the destructor before Test::finalize()
		Dhrystone dhrystone("dhrystone");
		dhrystone.execute(Test::EXECUTE_PERFORMANCE);
	}

	Test::finalize();

	return 0;
}
