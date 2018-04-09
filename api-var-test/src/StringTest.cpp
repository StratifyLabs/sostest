
#include <sapi/var.hpp>
#include "StringTest.hpp"
int StringTest::recursive_number = 0;

StringTest::StringTest() : Test("var::String"){

}
/*! \details Test  for api a var::String
 *  assign,find,compare,tolower,toupper operator(==,!=)
 * atoi,substr,inserts,erase
 * @return false if same test failed
 */

bool StringTest::execute_class_api_case(){
    bool result = true;
    if(!api_case_assign()){
        print_case_message("why", "String assing test failed");
        result = false;
    }else{
        print_case_message("why", "String assing test passed");
    }
    if(!api_case_compare()){
        print_case_message("why", "String compare test failed");
        result = false;
    }else{
        print_case_message("why", "String compare test passed");
    }
    if(!api_case_find()){
        print_case_message("why", "String find test failed");
        result = false;
    }else{
        print_case_message("why", "String find test passed");
    }
    if(!api_case_special()){
        print_case_message("why", "String special test failed");
        result = false;
    }else{
        print_case_message("why", "String special test passed");
    }
    return result;
}
/*! \details Test  for assing a var::String
 * @return false if same test failed
 */

bool StringTest::api_case_assign(){
    String s1;
    String s2;
    bool result = true;

    s1.assign("uno");
    if(s1.data() == 0 || s2.data()==0){
        print_case_message("why", "allocate memory failed");
        result = false;
    }
    s1.free();
    if(s1.data() != 0){
        print_case_message("why", "allocate memory exist after clear");   
        result = false;
    }
    s2.assign("123456789",3,5);
    s1 = s2;
    if(s1.data() == 0){
        result = false;
    }
    String s3;
    s3 = "string_lenth_15";
    s1.assign(s3.c_str(),s3.length()-3);
    if(s1.data() == 0){
        print_case_message("why", "allocate memory failed");
        result = false;
    }
    s1.append(s3.at(s3.length()-3));
    s1.append(s3.at(s3.length()-2));
    s1.append(s3.at(s3.length()-1));

    if(s1 != s3){
        print_case_message("why", "string not compare");
        result = false;
    }
    return result;
}
/*! \details Test  for compares a var::String
 * @return false if same test failed
 */
bool StringTest::api_case_compare(){
    bool result = true;
    String s1 = "123456789.987654321";
    const char s2[] = "123456789.987654321";
    const char s2_menos[] = "123456789.98765432";
    String s3 = "123456789.987654321";
    //with char compare
    if (s1.compare(s2)){
        print_case_message("compare char 1", "why not compare");
        result = false;
    }
    if (s1.compare(0,sizeof(s2),s2)){
        print_case_message("compare char 2", "why not compare");
        result = false;
    }
    if (s1.compare(0,sizeof(s2)-2,s2,sizeof(s2)-2)){
        print_case_message("compare char 3", "why not compare");
        result = false;
    }
    if (!s1.compare(s2_menos)){
        print_case_message("compare char 1", "why compare");
        result = false;
    }
    if (!s1.compare(0,sizeof(s2_menos),s2_menos)){
        print_case_message("compare char 2", "why compare");
        result = false;
    }
    if (!s1.compare(2,sizeof(s2_menos)-2,s2_menos,sizeof(s2_menos)-2)){
        print_case_message("compare char 3", "why compare");
        result = false;
    }

    //with String object compare
    if (s1.compare(s3)){
        print_case_message("compare string obj 1", "why not compare");
        result = false;
    }
    if (s1.compare(0,s3.len(),(String &) s3)){
        print_case_message("compare string obj 2", "why not compare");
        result = false;
    }
    if (s1.compare(2,s3.len()-2,s3,2,s1.length())){
        print_case_message("compare string obj 3", "why not compare");
        result = false;
    }
    String s_upper_case = "QWERTYUIOPASDFGHJKLZXCVBNM";
    String s_lower_case = "qwertyuiopasdfghjklzxcvbnm";
    s_lower_case.toupper();
    if (s_upper_case.compare(s_lower_case)){
        print_case_message("compare string obj 4", "after toupper not compare");
        result = false;
    }
    s_lower_case.tolower();
    if (!s_upper_case.compare(s_lower_case)){
        print_case_message("compare string obj 5", "after tolower compare");
        result = false;
    }
    return result;
}

/*! \details test "finds" a var::String
  * @return false if some test failed
 */
bool StringTest::api_case_find(){
    String s = "QWERTYUIOPASDFGHJKLZXCVBNM";
    String s1 = "ASD";
    String s2 = "QWE";
    String s3 = "BNM";
    char s1_char[] = "ASD";
    char s2_char[] = "QWE";
    char s3_char[] = "BNM";
    char* s4 = NULL ;
    bool result = true;

    if ( s.find(s1,0) == s.npos || s.find(s2,0) == s.npos || s.find(s3,0) == s.npos){
        print_case_message("find 1", "string object");
        result = false;
    }
    if ( s.find(s1_char,0) == s.npos || s.find(s2_char,0) == s.npos || s.find(s3_char,0) == s.npos){
        print_case_message("find 2", "string char");
        result = false;
    }
    if ( s.find("Q",0) == s.npos || s.find("M",s.len()-2) == s.npos || s.find("A",1) == s.npos){
        print_case_message("find 3", "did't find char");
        result = false;
    }
    if (s.find(s4,0)!=s.npos){
        print_case_message("find 4", "find with NULL char string");
        result = false;
    }

    if ( s.rfind(s1,0) == s.npos || s.rfind(s2,0) == s.npos || s.rfind(s3,0) == s.npos){
        print_case_message("rfind 1", "string object");
        result = false;
    }
    if ( s.rfind(s1_char,0) == s.npos || s.rfind(s2_char,0) == s.npos || s.rfind(s3_char,0) == s.npos){
        print_case_message("rfind 2", "string char");
        result = false;
    }
    if ( s.rfind("Q",0) == s.npos || s.rfind("M",s.len()-2) == s.npos || s.rfind("A",1) == s.npos){
        print_case_message("rfind 3", "did't rfind char");
        result = false;
    }
    if (s.rfind(s4,0)!=s.npos){
        print_case_message("rfind 4", "rfind with NULL char string");
        result = false;
    }
    return result;
}
/*! \details test "special" a var::String
 *  capacity,operator(=,<<,==,!=),atoi,substr,inserts,erase
  * @return false if some test failed
 */
bool StringTest::api_case_special(){
    String s_upper_case = "QWERTYUIOPASDFGHJKLZXCVBNM";
    String s_lower_case = "qwertyuiopasdfghjklzxcvbnm";
    char s_lower_case_c[] = "qwertyuiopasdfghjklzxcvbnm";
    String s_test;
    u32 capacity;
    bool result = true;
    capacity = s_upper_case.capacity();
    capacity+=4096;
    s_upper_case.set_capacity(capacity);
    if (s_upper_case.capacity()!= capacity){
        print_case_message("capacity", "set cappacity failed");
        result = false;
    }
    s_test = s_lower_case_c;
    if(strncmp(s_test.c_str(),s_lower_case_c,sizeof(s_lower_case_c))){
        print_case_message("operator=", "char to String failed");
        result = false;
    }
    s_upper_case = s_lower_case;
    if(strncmp(s_upper_case.c_str(),s_lower_case.c_str(),s_lower_case.length())){
        print_case_message("operator=", "char to String failed");
        result = false;
    }
    char c_append[] = "end";
    String s_append = "end";
    s_upper_case<<c_append;
    s_lower_case<<s_append;
    if(strncmp(s_upper_case.c_str(),s_lower_case.c_str(),s_lower_case.length())){
        print_case_message("operator<<", "failed");
        result = false;
    }

    if(s_upper_case!=s_lower_case){
        print_case_message("operator!=", "failed");
        result = false;
    }

    s_lower_case<<s_append;
    if(s_upper_case==s_lower_case){
        print_case_message("operator==", "failed");
        result = false;
    }
    String s_int = "47898";
    int i = 47898;
    if(s_int.atoi()!=i){
        print_case_message("atoi", "atoi %d failed",i);
        result = false;
    }
    s_int = "-47898";
    i = -47898;
    if(s_int.atoi()!=i){
        print_case_message("atoi", "atoi %d failed",i);
        result = false;
    }
    i++;
    if(s_int.atoi()==i){
        print_case_message("atoi", "atoi %d failed",i);
        result = false;
    }

    String s_float = "3.1";
    float f = 3.1;
    if(s_float.atoff()!=f){
        print_case_message("atoi", "atoi %f failed",f);
        result = false;
    }
    s_upper_case = s_lower_case;
    String s_sub_1 = s_upper_case.substr(3,4);
    String s_sub_2 = s_lower_case.substr(3,4);
    if(strncmp(s_sub_1.c_str(),s_sub_2.c_str(),s_lower_case.length())){
        print_case_message("substr", "failed");
        result = false;
    }

    String s_inserts = "qwertyuiopasdfghjkl";
    String s_inserts_con = "qwertyuiopasdfghjkl";
    String s_inserting = "123";
    s_inserts.insert(3,s_inserting.c_str());
    if(strncmp(s_inserts.c_str(),"qwe123rtyuiopasdfghjkl",s_inserting.length()+s_inserts.len())){
        print_case_message("insert", "failed");
        result = false;
    }
    s_inserts.erase(3,3);
    if(s_inserts_con.compare(s_inserts)){
        print_case_message("erase", "failed");
        result = false;
    }
    return result;
}
/*! \details test "stress" for var::String
 *  capacity,operator(=,<<,==,!=),atoi,substr,inserts,erase
  * @return false if some test failed
 */
bool StringTest::execute_class_performance_case(){
    bool result = true;
    int i;
    String t1 = "u";
    String t2 = "u";

    for(i=0; i < 1000; i++){
        if (t1.compare(t2)){
            print_case_message("performance 0", "simple compare failed %d",i);
            return false;
        }
        t1.append("d");
        t2.append("d");
        t1.toupper();
        t2.toupper();
        if(strncmp(t1.c_str(),t2.c_str(),t1.length())){
            print_case_message("performance 1", "compare after append failed %d",i);
            return false;
        }
        String s_sub_1 = t1.substr(t1.len()/2,t1.len()-1);
        String s_sub_2 = t1.substr(t1.len()/2,t1.len()-1);
        if (s_sub_1.compare(s_sub_2)){
            print_case_message("performance 2", "substring compare failed %d",i);
            return false;
        }
        if ( t1.find("D",0) == t1.npos || t2.rfind("D",0) == t2.npos){
            print_case_message("performance 3", "find failed %d",i);
            return false;
        }
    }

    return result;
}

bool StringTest::execute_class_stress_case(){
    bool result = true;
    for(u16 i = 0;i<1000;i++){
        String data("uno");
        recursive_number = 0;
        if(!execute_recursive(data)){
            print_case_message("req","reqursive test failed %d",recursive_number);
            result = false;
        }
    }
    return result;
}

bool StringTest::execute_recursive(String data){
    recursive_number++;
    if (data.calc_size() < 150){
        String data_new = data;
        data_new.append("uno");
        if(!execute_recursive(data_new)){
            return false;
        }
        u32 pos;
        pos = data_new.rfind("uno");
        if(pos == data_new.npos){
            return false;
        }
        data_new.erase(pos);
        if(data_new.compare(data)){
            return false;
        }
    }
    return true;
}



