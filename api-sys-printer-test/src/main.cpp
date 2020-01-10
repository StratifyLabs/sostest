#include <cstdio>
#include <sapi/sys.hpp>
#include <sapi/test.hpp>
#include <sapi/chrono.hpp>

#include "PrinterTest.hpp"
#include "MarkdownPrinterTest.hpp"
#include "YamlPrinterTest.hpp"

enum {
	PRINTER_TEST_FLAG = (1<<5),
	MARKDOWN_PRINTER_TEST_FLAG = (1<<6),
	YAML_PRINTER_TEST_FLAG = (1<<7),
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

	if( o_flags & PRINTER_TEST_FLAG ){
		PrinterTest test;
		test.execute(o_flags);
	}

	if( o_flags & MARKDOWN_PRINTER_TEST_FLAG ){
		MarkdownPrinterTest test;
		test.execute(o_flags);
	}

	if( o_flags & YAML_PRINTER_TEST_FLAG ){
		YamlPrinterTest test;
		test.execute(o_flags);
	}

	return 0;

}

u32 decode_cli(const Cli & cli){
	u32 o_flags = 0;

	o_flags |= Test::parse_execution_flags(cli);

	o_flags |= Test::parse_test(cli, "printer", PRINTER_TEST_FLAG);
	o_flags |= Test::parse_test(cli, "markdown", MARKDOWN_PRINTER_TEST_FLAG);
	o_flags |= Test::parse_test(cli, "yaml", YAML_PRINTER_TEST_FLAG);

	return o_flags;

}

void show_usage(const Cli & cli){
	cli.show_options();
}



