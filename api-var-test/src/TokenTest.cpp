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
	String s1("Uno,dos,tres, quatro or  cinko");
	Tokenizer one(
				s1.cstring(),
				Tokenizer::Delimeters(",. "),
				Tokenizer::IgnoreBetween(";")
				);
	if(one.size() != 6){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	String s[8];
	s[0] = "uno";
	s[1] = "dos";
	s[2] = "tres";
	s[3] = "quatro";
	s[4] = "cinko";
	s[5] = "seis";
	s[6] = "sieta";
	s[7] = "ocho";
	String s_common;
	//generate string for token
	for (u32 i =0;i<48;i++){
		s_common.append(s[i%8].cstring());
		if(i<16){
			s_common.append(",");
		}else if(i<32){
			s_common.append(";");
		}else if(i<47){
			s_common.append(" ");
		}
	}
	//parse string for and verify with base
	Tokenizer two(
				s_common.cstring(),
				Tokenizer::Delimeters(",; "),
				Tokenizer::IgnoreBetween("(")
				);

	for(u32 i =0;i<48;i++){

		TEST_THIS_EXPECT(String, two.at(i), s[i%8]);

	}
	for(u8 j=0;j<=0x0f;j++){
		char char_ascii;
		s_common.assign(s[0].cstring());
		//generate string
		for (u32 i =0;i<100;i++){
			s_common.append(s[i%8].cstring());
			//add character from "space" to "/" in ascii table
			char_ascii = 0x20 | j;
			s_common.append(char_ascii);
		}
		//parse string and verify
		char delim[2] = "\0";
		delim[0] = 0x20 | j;
		Tokenizer two(
					s_common.cstring(),
					Tokenizer::Delimeters(delim)
					);
		for(u32 i =0;i<100;i++){
			TEST_THIS_EXPECT(String,two.at(i), s[i%8]);
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
