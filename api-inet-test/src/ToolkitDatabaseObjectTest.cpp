#include "ToolkitDatabaseObjectTest.hpp"

#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include <CloudAPI/cloud.hpp>

ToolkitDatabaseObjectTest::ToolkitDatabaseObjectTest() : Test("ToolkitDatabaseObjectTest"){

}


bool ToolkitDatabaseObjectTest::execute_class_api_case(){


	print_case_message("start database object test");

	if( m_project.is_empty() ){
		print_case_failed("need to specify project name with --project=<name>");
		//return case_result();
	}

	//Cloud cloud("pureaire-a2f7e", "");
	Cloud cloud(m_project, "");
	DatabaseObject database_object(cloud);

	print_case_message("project name is %s", m_project.cstring());

	//need to login -- change the rules later
	//Sched::set_scheduler(Sched::get_pid(), Sched::FIFO, 16);

	//need to upload data
	database_object.to_object().insert("name", JsonString("Tyler Gilbert"));

	print_case_message("timestamp is %d", Time::get_time_of_day().time());

	String result = database_object.upload("test");

	if( result.is_empty() ){
		print_case_failed("failed to upload document -> %s", database_object.error_message().cstring());
		return case_result();
	}

	result = database_object.upload("test", "use_this_id");
	if( result.is_empty() ){
		print_case_failed("failed to upload document -> %s", database_object.error_message().cstring());
		return case_result();
	}

	if( result.is_empty() ){
		print_case_failed("failed to upload document -> %s", database_object.error_message().cstring());
		return case_result();
	}

	if( database_object.download("devices/00000000353833333036511900380039/application", "") < 0 ){
		print_case_failed("failed to download document -> %s", database_object.error_message().cstring());
		return case_result();
	} else {
		print_case_message("downloaded %d objects", database_object.to_object().count());
	}

	print_case_message("object -> '%s'", JsonDocument().stringify(database_object.to_object()).cstring());
	print_case_message("test was a success!!!");


	return case_result();
}
