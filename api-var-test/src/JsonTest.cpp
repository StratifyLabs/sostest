#include <sapi/var.hpp>
#include <sapi/sys.hpp>

#include "JsonTest.hpp"

JsonTest::JsonTest() : Test("JsonTest"){

}


bool JsonTest::execute_class_api_case(){

	if( JsonValue::api().is_valid() == false ){
		print_case_message("JSON API does not exist on this device...abort test");
		return case_result();
	}


	JsonObject object;
	object
			.insert("string", JsonString("string"))
			.insert("number", JsonInteger(0))
			.insert("real", JsonReal(0.0f))
			.insert("true", JsonTrue())
			.insert("false", JsonFalse())
			.insert("null", JsonNull());

	TEST_THIS_EXPECT(bool, object.at("string").to_string() == "string", true);
	TEST_THIS_EXPECT(bool, object.at("number").to_integer() == 0, true);
	TEST_THIS_EXPECT(bool, object.at("real").to_float() == 0.0f, true);
	TEST_THIS_EXPECT(bool, object.at("true").to_bool() == true, true);
	TEST_THIS_EXPECT(bool, object.at("false").to_bool() == false, true);
	TEST_THIS_EXPECT(bool, object.at("null").to_bool() == false, true);

	{
		JsonObject child_object;
		child_object
				.insert("string", JsonString("string"))
				.insert("number", JsonInteger(0))
				.insert("real", JsonReal(0.0f))
				.insert("true", JsonTrue())
				.insert("false", JsonFalse())
				.insert("null", JsonNull());

		object.insert("object", child_object);


		child_object = object.at("object");
		TEST_THIS_EXPECT(bool, child_object.at("string").to_string() == "string", true);
		TEST_THIS_EXPECT(bool, child_object.at("number").to_integer() == 0, true);
		TEST_THIS_EXPECT(bool, child_object.at("real").to_float() == 0.0f, true);
		TEST_THIS_EXPECT(bool, child_object.at("true").to_bool() == true, true);
		TEST_THIS_EXPECT(bool, child_object.at("false").to_bool() == false, true);
		TEST_THIS_EXPECT(bool, child_object.at("null").to_bool() == false, true);
	}

	{
		JsonArray child_array;
		for(u32 i=0; i < 10; i++){
			child_array.append(JsonString("string"));
			child_array.append(JsonTrue());
			child_array.append(JsonFalse());
			child_array.append(JsonInteger(0));
			child_array.append(JsonReal(0.0f));
			child_array.append(JsonNull());
		}
		object.insert("array", child_array);
		TEST_THIS_EXPECT(
					bool,
					object.at("array").to_array().count() == 10*6,
					true);
	}

	TEST_THIS_EXPECT(bool, object.at("string").to_string() == "string", true);
	TEST_THIS_EXPECT(bool, object.at("number").to_integer() == 0, true);
	TEST_THIS_EXPECT(bool, object.at("number").to_string() == "0", true);
	TEST_THIS_EXPECT(bool, object.at("real").to_float() == 0.0f, true);
	TEST_THIS_EXPECT(bool, object.at("true").to_string() == "true", true);
	TEST_THIS_EXPECT(bool, object.at("true").to_bool() == true, true);
	TEST_THIS_EXPECT(bool, object.at("true").to_integer() != 0, true);
	TEST_THIS_EXPECT(bool, object.at("false").to_string() == "false", true);
	TEST_THIS_EXPECT(bool, object.at("false").to_bool() == false, true);
	TEST_THIS_EXPECT(bool, object.at("false").to_integer() == 0, true);

	TEST_THIS_EXPECT(bool, object.at("array").to_bool() == true, true);
	TEST_THIS_EXPECT(bool, object.at("object").to_bool() == true, true);

	TEST_THIS_EXPECT(bool, object.at("object").is_array() == false, true);
	TEST_THIS_EXPECT(bool, object.at("object").is_true() == false, true);
	TEST_THIS_EXPECT(bool, object.at("object").is_false() == false, true);
	TEST_THIS_EXPECT(bool, object.at("object").is_null() == false, true);
	TEST_THIS_EXPECT(bool, object.at("object").is_integer() == false, true);
	TEST_THIS_EXPECT(bool, object.at("object").is_string() == false, true);
	TEST_THIS_EXPECT(bool, object.at("object").is_real() == false, true);
	TEST_THIS_ASSERT(bool, object.at("object").is_object() == true, true);

	TEST_THIS_EXPECT(bool, object.at("string").is_array() == false, true);
	TEST_THIS_EXPECT(bool, object.at("string").is_true() == false, true);
	TEST_THIS_EXPECT(bool, object.at("string").is_false() == false, true);
	TEST_THIS_EXPECT(bool, object.at("string").is_null() == false, true);
	TEST_THIS_EXPECT(bool, object.at("string").is_integer() == false, true);
	TEST_THIS_EXPECT(bool, object.at("string").is_string() == true, true);
	TEST_THIS_EXPECT(bool, object.at("string").is_real() == false, true);
	TEST_THIS_EXPECT(bool, object.at("string").is_object() == false, true);

	TEST_THIS_EXPECT(bool, object.at("number").is_array() == false, true);
	TEST_THIS_EXPECT(bool, object.at("number").is_true() == false, true);
	TEST_THIS_EXPECT(bool, object.at("number").is_false() == false, true);
	TEST_THIS_EXPECT(bool, object.at("number").is_null() == false, true);
	TEST_THIS_EXPECT(bool, object.at("number").is_integer() == true, true);
	TEST_THIS_EXPECT(bool, object.at("number").is_string() == false, true);
	TEST_THIS_EXPECT(bool, object.at("number").is_real() == false, true);
	TEST_THIS_EXPECT(bool, object.at("number").is_object() == false, true);

	TEST_THIS_EXPECT(bool, object.at("true").is_true() == true, true);
	TEST_THIS_EXPECT(bool, object.at("true").is_false() == false, true);
	TEST_THIS_EXPECT(bool, object.at("true").is_null() == false, true);
	TEST_THIS_EXPECT(bool, object.at("true").is_integer() == false, true);
	TEST_THIS_EXPECT(bool, object.at("true").is_string() == false, true);
	TEST_THIS_EXPECT(bool, object.at("true").is_real() == false, true);
	TEST_THIS_EXPECT(bool, object.at("true").is_array() == false, true);
	TEST_THIS_EXPECT(bool, object.at("true").is_object() == false, true);

	TEST_THIS_EXPECT(bool, object.at("false").is_array() == false, true);
	TEST_THIS_EXPECT(bool, object.at("false").is_true() == false, true);
	TEST_THIS_EXPECT(bool, object.at("false").is_false() == true, true);
	TEST_THIS_EXPECT(bool, object.at("false").is_null() == false, true);
	TEST_THIS_EXPECT(bool, object.at("false").is_integer() == false, true);
	TEST_THIS_EXPECT(bool, object.at("false").is_string() == false, true);
	TEST_THIS_EXPECT(bool, object.at("false").is_real() == false, true);
	TEST_THIS_EXPECT(bool, object.at("false").is_object() == false, true);

	{
		JsonObject child_object = object.at("object").to_object();
	}

	return case_result();
}

bool JsonTest::execute_class_performance_case(){
	return case_result();

}

bool JsonTest::execute_class_stress_case(){
	return case_result();
}
