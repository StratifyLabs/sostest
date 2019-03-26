#ifndef TOOLKITDATABASEOBJECTTEST_HPP
#define TOOLKITDATABASEOBJECTTEST_HPP

#include <sapi/test.hpp>
#include <sapi/var.hpp>


class ToolkitDatabaseObjectTest : public Test {
public:
	ToolkitDatabaseObjectTest();

	void set_project(const ConstString & project){
		m_project = project;
	}

	bool execute_class_api_case();

private:
	String m_project;

};

#endif // TOOLKITDATABASEOBJECTTEST_HPP
