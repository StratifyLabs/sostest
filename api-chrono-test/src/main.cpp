#include <stdio.h>

#include <sapi/sys.hpp>
#include <sapi/test.hpp>

#include "TimerTest.hpp"
#include "MicroTimeTest.hpp"
#include "TimeTest.hpp"

//update flags
enum {
	//Test flags occupy lower bit values
	MICRO_TIME_TEST_FLAG = (1<<5),
	TIMER_TEST_FLAG = (1<<6),
	TIME_TEST_FLAG = (1<<7)
};

u32 decode_cli(const Cli & cli);
void show_usage(const Cli & cli);

int main(int argc, char * argv[]){
	Cli cli(argc, argv);
	cli.set_publisher("Stratify Labs, Inc");
	cli.handle_version();

	u32 o_flags = decode_cli(cli);

	if( o_flags == 0 ){
		show_usage(cli);
		exit(0);
	}

   Test::initialize(
            Test::Name(cli.name()),
            Test::Version(cli.version()),
            Test::GitHash(SOS_GIT_HASH)
            );


	if( o_flags & TIMER_TEST_FLAG ){
		TimerTest test;
		test.execute(o_flags);
	}

	if( o_flags & MICRO_TIME_TEST_FLAG ){
		MicroTimeTest test;
		test.execute(o_flags);
	}

	if( o_flags & TIME_TEST_FLAG ){
		TimeTest test;
		test.execute(o_flags);
	}

	Test::finalize();
	return 0;

}

u32 decode_cli(const Cli & cli){
   u32 o_flags = 0;
   o_flags = Test::parse_execution_flags(cli);
   o_flags |= Test::parse_test(cli, "microTime", MICRO_TIME_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "timer", TIMER_TEST_FLAG);
   o_flags |= Test::parse_test(cli, "time", TIME_TEST_FLAG);
   return o_flags;


}

void show_usage(const Cli & cli){
   cli.show_options();
}










