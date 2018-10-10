#include <sapi/var.hpp>
#include <sapi/chrono.hpp>

#include "TokenTest.hpp"
int TokenTest::recursive_number = 0;

TokenTest::TokenTest(): Test("var::Token"){

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
			s_common.append(s[i%8].c_str());
			//add character from "space" to "/" in ascii table
			char char_ascii = 0x20 | (i & 0x0f);
			s_common.append(char_ascii);
		}
		//parse string and verify
		char delim[0x0f+2] = "\0";
		for (u8 i=0;i<=0x0f;i++){
			delim[i] = 0x20 | i;
		}
		Token two(s_common.c_str(),delim,0);
		for(u32 i =0;i<50;i++){
			if( two.at(i).compare(0, s[i%8].length(), s[i%8]) ){
				//if (strncmp(two.at(i),s[i%8].c_str(),s[i%8].len())){
				print_case_message("%s",delim);
				print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
				result = false;
				return result;
			}
		}
	}
	return result;
}

bool TokenTest::execute_class_stress_case(){
	bool result = true;
	String token_string;
	String delim_string;
	token_string.clear();
	print_case_message("Start recursive test:%s", token_string.c_str());
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
	Token one(s1.c_str(),",. ",";",0);
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
		s_common.append(s[i%8].c_str());
		if(i<16){
			s_common.append(",");
		}else if(i<32){
			s_common.append(";");
		}else if(i<47){
			s_common.append(" ");
		}
	}
	//parse string for and verify with base
	Token two(s_common.c_str(),",; ","(",0);
	for(u32 i =0;i<48;i++){
		if (strncmp(two.at(i).str(),s[i%8].str(),s[i%8].len())){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
		}
	}
	for(u8 j=0;j<=0x0f;j++){
		char char_ascii;
		s_common.assign(s[0].c_str());
		//generate string
		for (u32 i =0;i<100;i++){
			s_common.append(s[i%8].c_str());
			//add character from "space" to "/" in ascii table
			char_ascii = 0x20 | j;
			s_common.append(char_ascii);
		}
		//parse string and verify
		char delim[2] = "\0";
		delim[0] = 0x20 | j;
		Token two(s_common.c_str(),delim,0);
		for(u32 i =0;i<100;i++){
			if (strncmp(two.at(i).str(),s[i%8].c_str(),s[i%8].len())){
				print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
				result = false;
				break;
			}
		}
	}

	//add parse with not zero
	return result;
}

bool TokenTest::execute_recursive(String &token_string,String &delim_string){
	recursive_number++;
	u16 token_number = recursive_number;
	print_case_message("recursive token:%d", recursive_number);
	if(token_string.length() < 200){
		String tmp;
		tmp.sprintf("tmp%d", recursive_number);
		token_string.append(tmp.str());
		char delim = 0x20 | (recursive_number&0x0f);
		delim_string.append(delim);
		token_string.append(delim);
		Token new_token(token_string.c_str(),delim_string.c_str());
		if(!execute_recursive(token_string, delim_string)){
			//print_case_message("Failed %s:%d (%d)", __PRETTY_FUNCTION__, __LINE__, token_number);
			//return false;
		}
		if((new_token.count()) != token_number){
			print_case_message("Failed %s:%d (%d != %d)", __PRETTY_FUNCTION__, __LINE__, new_token.count(), token_number);
			return false;
		}
	} else {
		print_case_message("token count %lu",token_number);
	}

	return true;
}
