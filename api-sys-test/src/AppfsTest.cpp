#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>
#include <sapi/fmt.hpp>
#include "AppfsTest.hpp"
AppfsTest::AppfsTest() : Test("sys::Appfs"){

}
static bool make_file_in_flash(char *file_name);
static void rand_string_value(u16 size,String & string);
static u8 pseudo_rnd_u8() ;
static const u16 page_size = 32;

/*@brief api test for sys/appfs use "api-sys-test -appfs -api"
 * @warning
 * Appfs::create != 0
 */
bool AppfsTest::execute_class_api_case(){
    bool result = true;
    char file_name[] = "my_data_1";
    appfs_info_t info;
    u16 version;
    if(!make_file_in_flash(file_name)){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    char my_user_data[16];
    File f;
    String path = "/app/flash/";
    path.append(file_name);
    f.open(path.c_str(), File::RDONLY); //only the file read only
    f.read(my_user_data, 16); //read 16 bytes of user data
    f.close();  //free resources
    print_case_message("data %s",my_user_data);
    if(Appfs::get_info(path.c_str(),info)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }
    if(Appfs::get_info(file_name,info)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    version = Appfs::get_version(path.c_str());
    print_case_message("version %u",version);
    char id[16];
    if(Appfs::get_id(path.c_str(),id,page_size)!=0){
        print_case_message("Failed %s:%d", __PRETTY_FUNCTION__, __LINE__);
        result = false;
    }

    return result;
}
static bool make_file_in_flash(char *file_name){
    bool result = true;
    char my_user_data[16];
    //populate my_user_data as you please here
    sprintf(my_user_data, "Hello World 1!");
    File f;
    String path = "/app/flash/";
    path.append(file_name);
    if(f.access(path.c_str(),File::READ_OK)==0){
//file exist
    }else{
        if(Appfs::create(file_name, my_user_data, page_size)!=0){ //creates /app/flash/my_data as read only data
            result = false;
        }
    }
    return result;
}


/*@brief stress test for sys/appfs use "api-sys-test -appfs -stress"
 */
bool AppfsTest::execute_class_stress_case(){
    bool result = true;
    return result;
}

/*@brief performance test for sys/appfs use "api-sys-test -appfs -performance"
 */
bool AppfsTest::execute_class_performance_case(){
    bool result = true;
    return result;
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
#define BIT(n) (1<<n)
#define POLINOM_8BIT(ps_rnd) (((ps_rnd & BIT(7)) >> 7)^((ps_rnd & BIT(3)) >> 3)^((ps_rnd & BIT(2)) >> 2)^((ps_rnd & BIT(1)) >> 1)^1)
static u8 pseudo_rnd_u8() {
    u16 bit;
    static u16 ps_rnd = 1;
    bit = POLINOM_8BIT(ps_rnd);
    ps_rnd = ((ps_rnd << 1) + bit);
    ps_rnd &= 0xFF;
    return ps_rnd;
}
