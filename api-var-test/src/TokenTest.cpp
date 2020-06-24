#include <sapi/var.hpp>
#include <sapi/chrono.hpp>

#include "TokenTest.hpp"
int TokenTest::recursive_number = 0;

TokenTest::TokenTest(): Test("var::Token"){
	Data::reclaim_heap_space();
}

bool TokenTest::execute_class_performance_case(){
	bool result = true;
	String s[8];
	for(u32 j=0;j<1000;j++){
		s[0] = "uno";
		s[1] = "dos";
		s[2] = "tres";
		s[3] = "quatro";
		s[4] = "cinko";
		s[5] = "seis";
		s[6] = "sieta";
		s[7] = "ocho";
		String s_common;
		//generate string
		for (u32 i =0;i<50;i++){
			s_common.append(s[i%8].cstring());
			//add character from "space" to "/" in ascii table
			char char_ascii = 0x20 | (i & 0x0f);
			s_common.append(char_ascii);
		}
		//parse string and verify
		char delim[0x0f+2] = "\0";
		for (u8 i=0;i<=0x0f;i++){
			delim[i] = 0x20 | i;
		}
		Tokenizer two(
					s_common.cstring(),
					Tokenizer::Delimeters(delim)
					);
		for(u32 i =0;i<50;i++){

			TEST_THIS_EXPECT(int,
											 two.at(i).compare(
												 String::Position(0),
												 String::Length(s[i%8].length()),
											 s[i%8]), 0);

#if 0
			if( two.at(i).compare(0, s[i%8].length(), s[i%8]) ){
				//if (strncmp(two.at(i),s[i%8].cstring(),s[i%8].len())){
				print_case_message("%s",delim);
				print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
				result = false;
				return result;
			}
#endif
		}
	}
	return result;
}

bool TokenTest::execute_class_stress_case(){
	bool result = true;
	String token_string;
	String delim_string;
	token_string.clear();
	print_case_message("Start recursive test:%s", token_string.cstring());
	result = execute_recursive(token_string, delim_string);
	return result;
}
/*! \details test "api" a var::Token
 *  constructor,at(element),
 *  @return false if some test failed
 */
bool TokenTest::execute_class_api_case(){
	bool result = true;

	print_case_message("token API test");
	{
		const char * case_string = "test,1,2,3,4\n";
		{
			Tokenizer token_case(
						case_string,
						Tokenizer::Delimeters(",")
						);
			TEST_THIS_EXPECT(bool, token_case.count() == 5, true);
			TEST_THIS_EXPECT(bool, token_case.at(0) == "test", true);
			TEST_THIS_EXPECT(bool, token_case.at(1) == "1", true);
			TEST_THIS_EXPECT(bool, token_case.at(2) == "2", true);
			TEST_THIS_EXPECT(bool, token_case.at(3) == "3", true);
			TEST_THIS_EXPECT(bool, token_case.at(4) == "4\n", true);
		}
		{
			Tokenizer token_case(
						case_string,
						Tokenizer::Delimeters(",\n")
						);
			TEST_THIS_EXPECT(bool, token_case.count() == 6, true);
			TEST_THIS_EXPECT(bool, token_case.at(0) == "test", true);
			TEST_THIS_EXPECT(bool, token_case.at(1) == "1", true);
			TEST_THIS_EXPECT(bool, token_case.at(2) == "2", true);
			TEST_THIS_EXPECT(bool, token_case.at(3) == "3", true);
			TEST_THIS_EXPECT(bool, token_case.at(4) == "4", true);
			TEST_THIS_EXPECT(bool, token_case.at(5) == "", true);
		}
		{
			Tokenizer token_case(
						case_string,
						Tokenizer::Delimeters("\n")
						);
			TEST_THIS_EXPECT(bool, token_case.count() == 2, true);
			TEST_THIS_EXPECT(bool, token_case.at(0) == "test,1,2,3,4", true);
			TEST_THIS_EXPECT(bool, token_case.at(1) == "", true);
		}
	}

	{
		const char * case_string = "test,\"1,2\",3,4\n";
		{
			Tokenizer token_case(
						case_string,
						Tokenizer::Delimeters(","),
						Tokenizer::IgnoreBetween("\"")
						);
			TEST_THIS_EXPECT(bool, token_case.count() == 4, true);
			TEST_THIS_EXPECT(bool, token_case.at(0) == "test", true);
			TEST_THIS_EXPECT(bool, token_case.at(1) == "\"1,2\"", true);
			TEST_THIS_EXPECT(bool, token_case.at(2) == "3", true);
			TEST_THIS_EXPECT(bool, token_case.at(3) == "4\n", true);
		}
		{
			Tokenizer token_case(
						case_string,
						Tokenizer::Delimeters(",")
						);
			TEST_THIS_EXPECT(bool, token_case.count() == 5, true);
			TEST_THIS_EXPECT(bool, token_case.at(0) == "test", true);
			TEST_THIS_EXPECT(bool, token_case.at(1) == "\"1", true);
			TEST_THIS_EXPECT(bool, token_case.at(2) == "2\"", true);
			TEST_THIS_EXPECT(bool, token_case.at(3) == "3", true);
			TEST_THIS_EXPECT(bool, token_case.at(4) == "4\n", true);
		}
	}

	{
		const char * case_string = "test,'1','2','3,4',5\n";
		{
			Tokenizer token_case(
						case_string,
						Tokenizer::Delimeters(","),
						Tokenizer::IgnoreBetween("'")
						);
			TEST_THIS_EXPECT(bool, token_case.count() == 5, true);
			TEST_THIS_EXPECT(bool, token_case.at(0) == "test", true);
			TEST_THIS_EXPECT(bool, token_case.at(1) == "'1'", true);
			TEST_THIS_EXPECT(bool, token_case.at(2) == "'2'", true);
			TEST_THIS_EXPECT(bool, token_case.at(3) == "'3,4'", true);
			TEST_THIS_EXPECT(bool, token_case.at(4) == "5\n", true);
		}
	}

	{
		const char * case_string = "test,'1','2','','4','5'\n";
		{
			Tokenizer token_case(
						case_string,
						Tokenizer::Delimeters(",\n"),
						Tokenizer::IgnoreBetween("'")
						);
			TEST_THIS_EXPECT(bool, token_case.count() == 7, true);
			TEST_THIS_EXPECT(bool, token_case.at(0) == "test", true);
			TEST_THIS_EXPECT(bool, token_case.at(1) == "'1'", true);
			TEST_THIS_EXPECT(bool, token_case.at(2) == "'2'", true);
			TEST_THIS_EXPECT(bool, token_case.at(3) == "''", true);
			TEST_THIS_EXPECT(bool, token_case.at(4) == "'4'", true);
			TEST_THIS_EXPECT(bool, token_case.at(5) == "'5'", true);
			TEST_THIS_EXPECT(bool, token_case.at(6) == "", true);
		}
		{
			Tokenizer token_case(
						case_string,
						Tokenizer::Delimeters(",\n")
						);
			TEST_THIS_EXPECT(bool, token_case.count() == 7, true);
			TEST_THIS_EXPECT(bool, token_case.at(0) == "test", true);
			TEST_THIS_EXPECT(bool, token_case.at(1) == "'1'", true);
			TEST_THIS_EXPECT(bool, token_case.at(2) == "'2'", true);
			TEST_THIS_EXPECT(bool, token_case.at(3) == "''", true);
			TEST_THIS_EXPECT(bool, token_case.at(4) == "'4'", true);
			TEST_THIS_EXPECT(bool, token_case.at(5) == "'5'", true);
			TEST_THIS_EXPECT(bool, token_case.at(6) == "", true);
		}

		{
			Tokenizer token_case(
						case_string,
						Tokenizer::Delimeters(",")
						);
			TEST_THIS_EXPECT(bool, token_case.count() == 6, true);
			TEST_THIS_EXPECT(bool, token_case.at(0) == "test", true);
			TEST_THIS_EXPECT(bool, token_case.at(1) == "'1'", true);
			TEST_THIS_EXPECT(bool, token_case.at(2) == "'2'", true);
			TEST_THIS_EXPECT(bool, token_case.at(3) == "''", true);
			TEST_THIS_EXPECT(bool, token_case.at(4) == "'4'", true);
			TEST_THIS_EXPECT(bool, token_case.at(5) == "'5'\n", true);
		}
	}

	{
		const char * case_string = "app.install:path=HelloWorld,run,terminal,args='--test, --api, --token'";
		{
			Tokenizer token_case(
						case_string,
						Tokenizer::Delimeters(":"),
						Tokenizer::IgnoreBetween("'"),
						Tokenizer::MaximumCount(1)
						);

			TEST_THIS_ASSERT(bool, token_case.count() == 2, true);

			Tokenizer command(
						token_case.at(0),
						Tokenizer::Delimeters(".")
						);

			TEST_THIS_ASSERT(bool, command.count() == 2, true);
			TEST_THIS_EXPECT(bool, command.at(0) == "app", true);
			TEST_THIS_EXPECT(bool, command.at(1) == "install", true);


			Tokenizer arguments(
						token_case.at(1),
						Tokenizer::Delimeters(","),
						Tokenizer::IgnoreBetween("'")
						);

			TEST_THIS_ASSERT(bool, arguments.count() == 4, true);
			TEST_THIS_EXPECT(bool, arguments.at(0) == "path=HelloWorld", true);
			TEST_THIS_EXPECT(bool, arguments.at(1) == "run", true);
			TEST_THIS_EXPECT(bool, arguments.at(2) == "terminal", true);
			TEST_THIS_EXPECT(bool, arguments.at(3) == "args='--test, --api, --token'", true);

			for(u32 i=0; i < arguments.count(); i++){
				Tokenizer arg(
							arguments.at(i),
							Tokenizer::Delimeters("=")
							);

				if( i == 0 ){
					TEST_THIS_ASSERT(bool, arg.count() == 2, true);
					TEST_THIS_EXPECT(bool, arg.at(0) == "path", true);
					TEST_THIS_EXPECT(bool, arg.at(1) == "HelloWorld", true);
				} else if( i == 1 ){
					TEST_THIS_ASSERT(bool, arg.count() == 1, true);
					TEST_THIS_EXPECT(bool, arg.at(0) == "run", true);
				} else if( i == 2 ){
					TEST_THIS_ASSERT(bool, arg.count() == 1, true);
					TEST_THIS_EXPECT(bool, arg.at(0) == "terminal", true);
				} else if( i == 3 ){
					TEST_THIS_ASSERT(bool, arg.count() == 2, true);
					TEST_THIS_EXPECT(bool, arg.at(0) == "args", true);
					TEST_THIS_EXPECT(bool, arg.at(1) == "'--test, --api, --token'", true);
				}
			}
		}
	}

	{

		const char * command_reference =
				"copy:description=opt_string|The `filesystem.copy` command is used to copy files to/from the connected device to/from the host computer. Paths prefixed with *host@* are on the host computer while paths prefixed with *device@* are on the connected device.||filesystem.copy:source=host@README.md;dest=device@/app/flash/README.md|"
				",source_path=req_string|specifies the path to the source file to copy. If the source is a directory, all files in the directory are copied.||fs.copy:source=host@./data.txt,dest=device@/home/data.txt|"
				",destination_dest=req_string|specifies the destination path. If the source is a directory, the destination should be a directory as well.||fs.copy:source=host@./data.txt,dest=device@/home/data.txt|"
				",recursive=opt_bool_false|if the source is a directory it will be copied recursively.||fs.copy:source=host@data,dest=device@/home/data,recursive|"
				",remove=opt_bool_false|, if true, and if the source is on the device, the source will be deleted after it is copied.||fs.copy:source=host@./data.txt,dest=device@/home/data.txt,remove=true|"
				",hidden=opt_bool_false|specifies whether or not to copy hidden files (files that start with '.').||fs.copy:source=device@/home/directory,dest=soure@./,hidden=true|"
				",overwrite_o=opt_bool_true|, if true, the destination will be overwritten withouth warning.||fs.copy:source=host@./data.txt,dest=/home/data.txt,overwrite=false|";

		Tokenizer command_name_and_arguments(
					command_reference,
					Tokenizer::Delimeters(":"),
					Tokenizer::IgnoreBetween("|'\""),
					Tokenizer::MaximumCount(1));

		TEST_THIS_ASSERT(bool, command_name_and_arguments.count() == 2, true);
		TEST_THIS_EXPECT(bool, command_name_and_arguments.at(0) == "copy", true);

		Tokenizer arguments_tokens(
					command_name_and_arguments.at(1),
					Tokenizer::Delimeters(","),
					Tokenizer::IgnoreBetween("|'\"")
					);

		TEST_THIS_ASSERT(bool, arguments_tokens.count() == 7, true);
		TEST_THIS_EXPECT(bool, arguments_tokens.at(0) ==
										 "description=opt_string|The `filesystem.copy` command is used to copy files to/from the connected device to/from the host computer. Paths prefixed with *host@* are on the host computer while paths prefixed with *device@* are on the connected device.||filesystem.copy:source=host@README.md;dest=device@/app/flash/README.md|"
										 , true);
		TEST_THIS_EXPECT(bool, arguments_tokens.at(1) ==
										 "source_path=req_string|specifies the path to the source file to copy. If the source is a directory, all files in the directory are copied.||fs.copy:source=host@./data.txt,dest=device@/home/data.txt|"
										 , true);
		TEST_THIS_EXPECT(bool, arguments_tokens.at(2) ==
										 "destination_dest=req_string|specifies the destination path. If the source is a directory, the destination should be a directory as well.||fs.copy:source=host@./data.txt,dest=device@/home/data.txt|"
										 , true);
		TEST_THIS_EXPECT(bool, arguments_tokens.at(3) ==
										 "recursive=opt_bool_false|if the source is a directory it will be copied recursively.||fs.copy:source=host@data,dest=device@/home/data,recursive|"
										 , true);
		TEST_THIS_EXPECT(bool, arguments_tokens.at(4) ==
										 "remove=opt_bool_false|, if true, and if the source is on the device, the source will be deleted after it is copied.||fs.copy:source=host@./data.txt,dest=device@/home/data.txt,remove=true|"
										 , true);
		TEST_THIS_EXPECT(bool, arguments_tokens.at(5) ==
										 "hidden=opt_bool_false|specifies whether or not to copy hidden files (files that start with '.').||fs.copy:source=device@/home/directory,dest=soure@./,hidden=true|"
										 , true);
		TEST_THIS_EXPECT(bool, arguments_tokens.at(6) ==
										 "overwrite_o=opt_bool_true|, if true, the destination will be overwritten withouth warning.||fs.copy:source=host@./data.txt,dest=/home/data.txt,overwrite=false|"
										 , true);

		for(u32 i=0; i < arguments_tokens.count(); i++){
			Tokenizer arguments_values_and_descriptions(
						arguments_tokens.at(i),
						Tokenizer::Delimeters("|")
						);

			TEST_THIS_ASSERT(bool, arguments_values_and_descriptions.count() == 5, true);


			Tokenizer arguments_values(
						arguments_values_and_descriptions.at(0),
						Tokenizer::Delimeters("="),
						Tokenizer::IgnoreBetween("'\""),
						Tokenizer::MaximumCount(1)
						);

			TEST_THIS_ASSERT(bool, arguments_values_and_descriptions.count() >= 2, true);


		}


	}


	//add parse with not zero
	return result;
}

bool TokenTest::execute_recursive(String &token_string,String &delim_string){
	recursive_number++;
	u32 token_number = recursive_number;
	print_case_message("recursive token:%d", recursive_number);
	if(token_string.length() < 200){
		String tmp;
		tmp.format("tmp%d", recursive_number);
		token_string.append(tmp);
		char delim = 0x20 | (recursive_number&0x0f);
		delim_string.append(delim);
		token_string.append(delim);
		Tokenizer new_token(
					token_string,
					Tokenizer::Delimeters(delim_string)
					);
		if(!execute_recursive(token_string, delim_string)){
			//print_case_message("Failed %s:%d (%d)", __PRETTY_FUNCTION__, __LINE__, token_number);
			//return false;
		}

		TEST_THIS_EXPECT(u32, new_token.count(), token_number);

		if((new_token.count()) != token_number){
			print_case_message("Failed %s:%d (%d != %d)", __PRETTY_FUNCTION__, __LINE__, new_token.count(), token_number);
			return false;
		}
	} else {
		print_case_message("token count %lu",token_number);
	}

	return true;
}
