#ifndef HTTPCLIENTTEST_HPP
#define HTTPCLIENTTEST_HPP

#include <sapi/inet.hpp>
#include <sapi/test.hpp>

class HttpClientTest : public Test {
public:
	HttpClientTest();

	bool execute_class_api_case();

};


#endif // HTTPCLIENTTEST_HPP
