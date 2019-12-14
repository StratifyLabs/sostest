#ifndef SOCKETTEST_HPP
#define SOCKETTEST_HPP
#include <sapi/inet.hpp>
#include <sapi/test.hpp>

class SocketTest : public Test {
public:
	SocketTest();

	bool execute_class_api_case();
	bool execute_class_stress_case();

private:


};

#endif // SOCKETTEST_HPP
