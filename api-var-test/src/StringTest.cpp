
#include <sapi/var.hpp>
#include "StringTest.hpp"
int StringTest::recursive_number = 0;
static void rand_string_value(u16 size,String & string);

StringTest::StringTest() : Test("var::String"){
	Data::reclaim_heap_space();
}
/*! \details Test  for api a var::String use "api-var-test -string -api"
 *  assign,find,compare,tolower,toupper operator(==,!=)
 * atoi,substr,inserts,erase
 * @return false if same test failed
 * @todo add get_delimited_data
 */

bool StringTest::execute_class_api_case(){
	bool result = true;
	if(!api_case_assign()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}else{
		print_case_message("String assign test passed");
	}
	if(!api_case_compare()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}else{
		print_case_message("String compare test passed");
	}
	if(!api_case_find()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}else{
		print_case_message("String find test passed");
	}
	if(!api_case_special()){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}else{
		print_case_message("String special test passed");
	}
	return result;
}
/*! \details Test  for assign a var::String
 * @return false if same test failed
 */

bool StringTest::api_case_assign(){
	String s1;
	String s2;
	bool result = true;

	s1.assign("uno");
	TEST_THIS_EXPECT(String, s1, "uno");
	s1.clear();
	TEST_THIS_EXPECT_NOT(String, s1, String("uno"));
	TEST_THIS_EXPECT_NOT(String, s1.cstring(), nullptr);

	s2.assign("123456789");
	TEST_THIS_EXPECT(String, s2, "123456789");
	TEST_THIS_EXPECT_NOT(String, s1, s2);
	TEST_THIS_ASSERT_NOT(const char*, s1.cstring(), nullptr);

	s2.assign(s1);
	TEST_THIS_EXPECT(String, s1, s2);

	return result;
}
/*! \details Test  for compares a var::String
 * @return false if same test failed
 */
bool StringTest::api_case_compare(){
	bool result = true;
	String s1("123456789.987654321");
	const char s2[] = "123456789.987654321";
	const char s2_menos[] = "123456789.98765432";
	String s3("123456789.987654321");
	//with char compare

	TEST_THIS_EXPECT(int, s1.compare(s2), 0);
	TEST_THIS_EXPECT(int, s1.compare(String::Position(0),String::Length(sizeof(s2)),s2), 0);
	TEST_THIS_EXPECT(int, s1.compare(
							  String::Position(0),
							  String::Length(sizeof(s2)-2),
							  s2,
							  String::SubPosition(0),
							  String::SubLength(sizeof(s2)-2)
							  ),
						  0);

	TEST_THIS_EXPECT(int, s1.compare(
							  String::Position(2),
							  String::Length(sizeof(s2)-4),
							  s2,
							  String::SubPosition(2),
							  String::SubLength(sizeof(s2)-4)
							  ),
						  0);

	TEST_THIS_EXPECT(String, s1, s2_menos);
	TEST_THIS_EXPECT(int, s1.compare(String::Position(0),String::Length(sizeof(s2_menos)),s2_menos), 0);

	TEST_THIS_EXPECT(int, s1.compare(
							  String::Position(2),
							  String::Length(sizeof(s2_menos)-4),
							  s2_menos,
							  String::SubPosition(2),
							  String::SubLength(sizeof(s2_menos)-4)
							  ),
						  0);


	TEST_THIS_EXPECT(int, s1.compare(s3), 0);
	TEST_THIS_EXPECT(int, s1.compare(
							  String::Position(0),
							  String::Length(s3.length()),
							  s3
							  ), 0);

	TEST_THIS_EXPECT(int, s1.compare(
							  String::Position(2),
							  String::Length(s3.length()-2),
							  s3,
							  String::SubPosition(2),
							  String::SubLength(s3.length()-2)
							  ), 0);

	String s_upper_case("QWERTYUIOPASDFGHJKLZXCVBNM");
	String s_lower_case("qwertyuiopasdfghjklzxcvbnm");
	s_lower_case.to_upper();

	TEST_THIS_EXPECT(String, s_upper_case, s_lower_case);
	s_lower_case.to_lower();

	TEST_THIS_EXPECT_NOT(String, s_upper_case, s_lower_case);
	return result;
}

/*! \details test "finds" a var::String
  * @return false if some test failed
 */
bool StringTest::api_case_find(){
	String s("QWERTYUIOPASDFGHJKLZXCVBNM");
	String s1("ASD");
	String s2("QWE");
	String s3("BNM");
	char s1_char[] = "ASD";
	char s2_char[] = "QWE";
	char s3_char[] = "BNM";
	char* s4 = nullptr ;
	bool result = true;

	TEST_THIS_EXPECT(size_t, s.find(s1), 10);
	TEST_THIS_EXPECT(size_t, s.find(s2), 0);
	TEST_THIS_EXPECT(size_t, s.find(s3), 24);
	TEST_THIS_EXPECT(size_t, s.find(s1_char), 10);
	TEST_THIS_EXPECT(size_t, s.find(s2_char), 0);
	TEST_THIS_EXPECT(size_t, s.find(s3_char), 24);

#if 0
	if ( s.find(s1,0) == s.npos || s.find(s2,0) == s.npos || s.find(s3,0) == s.npos){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if ( s.find(s1_char,0) == s.npos || s.find(s2_char,0) == s.npos || s.find(s3_char,0) == s.npos){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if ( s.find("Q",0) == s.npos || s.find("M",s.length()-2) == s.npos || s.find("A",1) == s.npos){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if (s.find(s4,0)!=s.npos){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}

	if ( s.rfind(s1,0) == s.npos || s.rfind(s2,0) == s.npos || s.rfind(s3,0) == s.npos){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if ( s.rfind(s1_char,0) == s.npos || s.rfind(s2_char,0) == s.npos || s.rfind(s3_char,0) == s.npos){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if ( s.rfind("Q",0) == s.npos || s.rfind("M",s.length()-2) == s.npos || s.rfind("A",1) == s.npos){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	if (s.rfind(s4,0)!=s.npos){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
#endif
	return result;
}
/*! \details test "special" a var::String
 *  capacity,operator(=,<<,==,!=),atoi,substr,inserts,erase
  * @return false if some test failed
 */
bool StringTest::api_case_special(){
	String s_upper_case("QWERTYUIOPASDFGHJKLZXCVBNM");
	String s_lower_case("qwertyuiopasdfghjklzxcvbnm");
	char s_lower_case_c[] = "qwertyuiopasdfghjklzxcvbnm";
	String s_test;
	u32 capacity;
	bool result = true;
	capacity = s_upper_case.capacity();
	capacity+=4096;


	TEST_THIS_EXPECT(String, String(s_lower_case).to_upper(), s_upper_case);
	TEST_THIS_EXPECT(String, String(s_upper_case).to_lower(), s_lower_case);
	TEST_THIS_EXPECT_NOT(String, s_upper_case, s_lower_case);
	TEST_THIS_EXPECT_NOT(String, s_lower_case, s_upper_case);

	TEST_THIS_EXPECT(int, String("47898").to_integer(), 47898);
	TEST_THIS_EXPECT(int, String("4.7898").to_float(), 4.7898f);
	TEST_THIS_EXPECT(int, String("10").to_unsigned_long(String::base_10), 10);
	TEST_THIS_EXPECT(int, String("10").to_unsigned_long(String::base_8), 010);
	TEST_THIS_EXPECT(int, String("10").to_unsigned_long(String::base_16), 0x10);

#if 0

	s_upper_case.set_capacity(capacity);
	if (s_upper_case.capacity() < capacity){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	s_test = s_lower_case_c;
	if(strncmp(s_test.cstring(),s_lower_case_c,sizeof(s_lower_case_c))){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	s_upper_case = s_lower_case;
	if(strncmp(s_upper_case.cstring(),s_lower_case.cstring(),s_lower_case.length())){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	char c_append[] = "end";
	String s_append("end");
	s_upper_case<<c_append;
	s_lower_case<<s_append;
	if(strncmp(s_upper_case.cstring(),s_lower_case.cstring(),s_lower_case.length())){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}

	if(s_upper_case!=s_lower_case){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}

	s_lower_case<<s_append;
	if(s_upper_case==s_lower_case){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	String s_int("47898");
	int i = 47898;
	if(s_int.atoi()!=i){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	s_int = "-47898";
	i = -47898;
	if(s_int.atoi()!=i){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	i++;
	if(s_int.atoi()==i){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}

	String s_float("3.1");
	float f = 3.1;
	if(s_float.atoff()!=f){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	s_upper_case = s_lower_case;
	String s_sub_1 = s_upper_case.substr(3,4);
	String s_sub_2 = s_lower_case.substr(3,4);
	if(strncmp(s_sub_1.cstring(),s_sub_2.cstring(),s_lower_case.length())){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}

	String s_inserts("qwertyuiopasdfghjkl");
	String s_inserts_con("qwertyuiopasdfghjkl");
	String s_inserting("123");
	s_inserts.insert(3,s_inserting.cstring());
	if(strncmp(s_inserts.cstring(),"qwe123rtyuiopasdfghjkl",s_inserting.length()+s_inserts.length())){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	s_inserts.erase(3,3);
	if(s_inserts_con.compare(s_inserts)){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	u32 train_len;
	String s_train(ConstString("train:;;"),7);
	train_len = s_train.length();
	String s_wagon(":wagon:");
	for (i=0;i<10;i++){
		s_train.insert(s_train.length()-2,s_wagon.cstring());
		train_len += s_wagon.length();
		if(s_train.length()!=train_len){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
			print_case_message("failed on stm32f429zi %d != %d",  s_train.length(),train_len);
			result = false;
			break;
		}
	}
	train_len = s_train.length();
	for (;i;i--){
		s_train.erase(6,s_wagon.length());
		train_len -= s_wagon.length();
		if(s_train.length()!=train_len){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, i);
			result =false;
			break;
		}
	}
	String base("abcdefgfedcba");
	String base1("abcdefgfedcba");
	//insert impossible
	base1.insert(base1.length()+1, "ABC");
	if(strncmp(base.cstring(),base1.cstring(),base1.length())){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	base1.insert(5200, "ABC");
	if(strncmp(base.cstring(),base1.cstring(),base1.length())){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	//last
	base1 = base;
	base.append("ABC");
	base1.insert(base1.length(), "ABC");
	if(base.compare(base1)){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	//first
	base1 = base;
	String base_ABC("ABC");
	base_ABC.append(base.cstring());
	base1.insert(0, "ABC");
	if(base_ABC.compare(base1)){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	//midle
	String base_midle;
	base_midle = base;
	for (u32 j=1;j<base.length()-1;j++){
		base_midle.insert(j,base_ABC.cstring());
		base_midle.erase(j,base_ABC.length());
		if(base_midle.compare(base)){
			print_case_message("Failed %s:%d %d -> ('%s' != '%s')",
									 __PRETTY_FUNCTION__,
									 __LINE__,
									 j,
									 base_midle.cstring(),
									 base.cstring());
			result = false;
		}
	}
	//append char
	String base_b("b");
	for (u32 i =0;i<5;i++){
		base_b.append("b");
	}
	String base_inserts("a");
	for (u32 i =0;i<5;i++){
		base_inserts.append("a");
	}
	base_b.insert(1,base_inserts.cstring());
	if(base_b.compare("baaaaaabbbbb")){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	base_b.insert(2,base_inserts.cstring());
	base_b.erase(2,base_inserts.length());
	if(base_b.compare("baaaaaabbbbb")){
		print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
		result = false;
	}
	for (u32 i=1;i<base_b.length();i++){
		base_b.insert(i,base_inserts.cstring());
		base_b.erase(i,base_inserts.length());
		if(base_b.compare("baaaaaabbbbb")){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
		}
	}
	String base_c;
	base_c = base_b;
	//insert an erase

	for (u32 i=1;i<base_b.length();i++){
		base_b.insert(i,base_inserts.cstring());
		base_b.erase(i,base_inserts.length());
		if(base_b.compare(base_c.cstring())){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
		}
	}
#endif

	return result;
}
/*! \details test "performance" for var::String
 *  capacity,operator(=,<<,==,!=),atoi,substr,inserts,erase
  * @return false if some test failed
 */
bool StringTest::execute_class_performance_case(){
	bool result = true;
#if 0
	u32 i;
	String t1("u");
	String t2("u");


	for(i=0; i < 500; i++){
		if (t1.compare(t2)){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			return false;
		}
		t1.append("d");
		t2.append("d");
		t1.to_upper();
		t2.to_upper();
		if(strncmp(t1.cstring(),t2.cstring(),t1.length())){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			return false;
		}{
			String s_sub_1 = t1.create_sub_string(t1.length()/2,t1.length()-1);
			String s_sub_2 = t1.create_sub_string(t1.length()/2,t1.length()-1);
			if (s_sub_1.compare(s_sub_2)){
				print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
				return false;
			}
			if ( t1.find("D") == t1.npos || t2.rfind("D") == t2.npos){
				print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
				return false;
			}
		}
	}
	t1.~String();
	t2.~String();
	//append char
	String inserts("a");
	String base("b");
	String base_c("c");
	//base value is const
	u32 max_insert_value = 0;
	for (u32 i =0;i<250;i++){
		base.append("b");
	}
	for(u32 j = 1;j<100;j++){
		inserts = "a";
		for (u32 i =0;i<j;i++){
			inserts.append("a");
		}
		base_c = base;
		//insert and erase
		for (u32 i=1;i<base.length();i++){
			base.insert(String::Position(i),inserts.cstring());
			base.erase(String::Position(i),inserts.length());
			if(base.compare(base_c.cstring())){
				max_insert_value = j;
				result = false;
				break;
			}
		}
		if(max_insert_value){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, max_insert_value);
			result = false;
			break;
		}
	}
	//insert value is const
	inserts = "a";
	for (u32 i =0;i<max_insert_value;i++){
		inserts.append("a");
	}
	u32 max_base_value = 0;
	for(u32 j = 1;j<200;j++){
		base = "b";
		for (u32 i =0;i<j;i++){
			base.append("b");
		}
		base_c = base;
		//insert and erase
		for (u32 i=1;i<base.length();i++){
			base.insert(i,inserts.cstring());
			base.erase(i,inserts.length());
			if(base.compare(base_c.cstring())){
				max_base_value = j;
				result = false;
				break;
			}
		}
		if(max_base_value){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, max_base_value);
			result = false;
			break;
		}
	}
#endif

	return result;
}
/*! \details test "stress" for var::String
 *  capacity,operator(=,<<,==,!=),atoi,substr,inserts,erase
  * @return false if some test failed
  * @warning 4251501260:23.2:804A508--CRITICAL--Heap Fault if use rand_string_value(1,temp_string);
 */

bool StringTest::execute_class_stress_case(){
	bool result = true;
#if 0
	u32 i;
	String t1("u");
	String t2("u");
	String temp_string;
	//corner test
	for(i=0; i < 100; i++){
		if (t1.compare(t2)){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			return false;
		}
		rand_string_value(1,temp_string);
		t1.append(temp_string.at(0));
		t2.append(temp_string.at(0));
		if(strncmp(t1.cstring(),t2.cstring(),t1.length())){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			return false;
		}
		{
			String s_sub_1 = t1.substr(t1.length()/2,t1.length()-1);
			String s_sub_2 = t1.substr(t1.length()/2,t1.length()-1);
			if (s_sub_1.compare(s_sub_2)){
				print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
				return false;
			}
		}
		t1.toupper();
		t2.toupper();
		if(strncmp(t1.cstring(),t2.cstring(),t1.length())){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			return false;
		}
	}
	t1.~String();
	t2.~String();
	//append char
	rand_string_value(1,temp_string);
	String inserts(temp_string.cstring());
	rand_string_value(1,temp_string);
	String base(temp_string.cstring());
	rand_string_value(1,temp_string);
	String base_c(temp_string.cstring());
	//base value is const
	u32 max_insert_value = 0;
	for (u32 i =0;i<100;i++){
		base.append("b");
	}
	for(u32 j = 1;j<10;j++){
		rand_string_value(1,temp_string);
		inserts = temp_string.cstring();
		for (u32 i =0;i<j;i++){
			rand_string_value(1,temp_string);
			inserts.append(temp_string.cstring());
		}
		base_c = base;
		//insert and erase
		for (u32 i=1;i<base.length();i++){
			base.insert(i,inserts.cstring());
			base.erase(i,inserts.length());
			if(base.compare(base_c.cstring())){
				max_insert_value = j;
				result = false;
				break;
			}
		}
		if(max_insert_value){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, max_insert_value);
			result = false;
			break;
		}
	}
	//insert value is const
	inserts = "a";
	for (u32 i =0;i<max_insert_value;i++){
		inserts.append("a");
	}
	u32 max_base_value = 0;
	for(u32 j = 1;j<200;j++){
		base = "b";
		for (u32 i =0;i<j;i++){
			base.append("b");
		}
		base_c = base;
		//insert and erase
		for (u32 i=1;i<base.length();i++){
			base.insert(i,inserts.cstring());
			base.erase(i,inserts.length());
			if(base.compare(base_c.cstring())){
				max_base_value = j;
				result = false;
				break;
			}
		}
		if(max_base_value){
			print_case_message("Failed in cycle %s:%d:%d", __PRETTY_FUNCTION__, __LINE__, max_base_value);
			result = false;
			break;
		}
	}
	//recursive test
	for(u16 i = 0;i<100;i++){
		String data("uno");
		recursive_number = 0;
		if(!execute_recursive(data)){
			print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
			result = false;
		}
	}
#endif
	return result;
}

bool StringTest::execute_recursive(String data){
	recursive_number++;
	if (data.length() < 150){
		String data_new = data;
		data_new.append("uno");
		if(!execute_recursive(data_new)){
			return false;
		}
		size_t pos;
		pos = data_new.rfind("uno");
		if(pos == data_new.npos){
			return false;
		}
		data_new.erase(String::Position(pos));
		if(data_new.compare(data)){
			return false;
		}
	}
	return true;
}

static void rand_string_value(u16 size,String & string){
	string.clear();
	for (u16 i =0;i<size;i++){
		u8 value;
		value = (u8)(rand()%25);
		value +=97;
		string.append(value);
	}
}



