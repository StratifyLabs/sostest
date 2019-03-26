#include <string>
#include "StringTest.hpp"

using namespace std;

StringTest::StringTest() : Test("StringTest"){

}


bool StringTest::execute_class_api_case(){
	string one("one");
	string two("two");
	string sample_string;

	sample_string = "this is a sample string";
	print_case_message("Sample string '%s'", sample_string.c_str());

	if( one == two ){
		print_case_failed("'%s' == '%s'", one.c_str(), two.c_str());
	}

	one = two;
	if( one != two ){
		print_case_failed("'%s' != '%s'", one.c_str(), two.c_str());
	}

	return case_result();
}
