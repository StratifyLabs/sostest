#include <cstdio>
#include <sapi/sys.hpp>
#include <sapi/test.hpp>
#include <sapi/chrono.hpp>

#include "DataTest.hpp"
#include "QueueTest.hpp"
#include "StringTest.hpp"
#include "TokenTest.hpp"
#include "VectorTest.hpp"
#include "ArrayTest.hpp"
#include "RingTest.hpp"
#include "JsonTest.hpp"

enum {
	DATA_TEST_FLAG = (1<<5),
	QUEUE_TEST_FLAG = (1<<6),
	STRING_TEST_FLAG = (1<<7),
	TOKEN_TEST_FLAG = (1<<8),
	VECTOR_TEST_FLAG = (1<<9),
	ARRAY_TEST_FLAG = (1<<10),
	RING_TEST_FLAG = (1<<11),
	JSON_TEST_FLAG = (1<<12),
};

u32 decode_cli(const Cli & cli);
void show_usage(const Cli & cli);

int main(int argc, char * argv[]){
	Cli cli(argc, argv, SOS_GIT_HASH);
	cli.set_publisher("Stratify Labs, Inc");
	cli.handle_version();
	u32 o_flags;

	o_flags = decode_cli(cli);

	if( o_flags == 0 ){
		show_usage(cli);
		exit(0);
	}

	srand(Clock::get_time().nanoseconds());

	Test::initialize(
				Test::Name(cli.name()),
				Test::Version(cli.version()),
				Test::GitHash(SOS_GIT_HASH)
				);

	if( o_flags & DATA_TEST_FLAG ){
		DataTest test;
		test.execute(o_flags);
	}

	if( o_flags & QUEUE_TEST_FLAG ){
		QueueTest test;
		test.execute(o_flags);
	}

	if( o_flags & STRING_TEST_FLAG ){
		StringTest test;
		test.execute(o_flags);
	}

	if( o_flags & VECTOR_TEST_FLAG ){
		VectorTest test;
		test.execute(o_flags);
	}

	if( o_flags & TOKEN_TEST_FLAG ){
		TokenTest test;
		test.execute(o_flags);
	}

	if( o_flags & RING_TEST_FLAG ){
		RingTest test;
		test.execute(o_flags);
	}

	if( o_flags & ARRAY_TEST_FLAG ){
		ArrayTest test;
		test.execute(o_flags);
	}

	if( o_flags & JSON_TEST_FLAG ){
		JsonTest test;
		test.execute(o_flags);
	}


	Test::finalize();
	return 0;

}

u32 decode_cli(const Cli & cli){
	u32 o_flags = 0;
	o_flags = Test::parse_execution_flags(cli);
	o_flags |= Test::parse_test(cli, "data", DATA_TEST_FLAG);
	o_flags |= Test::parse_test(cli, "queue", QUEUE_TEST_FLAG);
	o_flags |= Test::parse_test(cli, "string", STRING_TEST_FLAG);
	o_flags |= Test::parse_test(cli, "token", TOKEN_TEST_FLAG);
	o_flags |= Test::parse_test(cli, "vector", VECTOR_TEST_FLAG);
	o_flags |= Test::parse_test(cli, "ring", RING_TEST_FLAG);
	o_flags |= Test::parse_test(cli, "array", ARRAY_TEST_FLAG);
	o_flags |= Test::parse_test(cli, "json", JSON_TEST_FLAG);
	return o_flags;

}

void show_usage(const Cli & cli){
	cli.show_options();
}



