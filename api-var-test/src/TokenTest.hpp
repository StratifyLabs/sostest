#ifndef TOKENTEST_HPP
#define TOKENTEST_HPP

#include <sapi/test.hpp>

class TokenTest : public Test {
public:
	TokenTest();

private:
	//cases
	bool execute_class_api_case();
	bool execute_class_performance_case();
	bool execute_class_stress_case();
	bool execute_recursive(String &token_string,String &delim_string);
	static int recursive_number;

};

#endif // TOKENTEST_HPP
