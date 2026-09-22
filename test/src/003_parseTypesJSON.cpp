/*********************************************************************************************
*	Name        : 003_perseTypesJSON.cpp
*  Description : Test parsing all types of the JSON
********************************************************************************************/

#include <string>
#include <gtest/gtest.h>

#include "libJSON.h"
#include "JSONParseUchar.h"


static const std::string kTestDataDir = TEST_DATA_DIR;


TEST (LibJSON, TestJSONTypes)
{
	JSONObject jobj;


	ASSERT_TRUE (JSON_ERR_CODE::SUCCESS == JSONParser::parseFromFile (jobj, (kTestDataDir + "/withAllDataTypes.json").c_str ()));


	//Existing fields
	EXPECT_TRUE (jobj.get ("id")->getType () == JSON_TYPE::JSTRING);
	EXPECT_TRUE (jobj.get ("name")->getType () == JSON_TYPE::JSTRING);
	EXPECT_TRUE (jobj.get ("isYummy")->getType () == JSON_TYPE::JBOOL);
	EXPECT_TRUE (jobj.get ("stock")->getType () == JSON_TYPE::JINT);
	EXPECT_TRUE (jobj.get ("price")->getType () == JSON_TYPE::JDOUBLE);
	EXPECT_TRUE (jobj.get ("lastOvenError")->getType () == JSON_TYPE::JNULL);
	EXPECT_TRUE (jobj.get ("images")->getType () == JSON_TYPE::JARR);
	EXPECT_TRUE (jobj.get ("visual")->getType () == JSON_TYPE::JOBJ);

	//Not existing fiels
	EXPECT_TRUE (jobj.get ("noExists")->getType () == JSON_TYPE::JNULL);

}




TEST (LibJSON, TestEmbeddedUnicode)
{
	//Check Parse one unicode char
	{
		std::string ucharStr = "00F1";
		std::string utf8Str = "\xC3\xB1";

		std::string parsed = JSONParseUchar::getUTF8Char (ucharStr);

		EXPECT_TRUE (parsed.compare (utf8Str) == 0);
	}


	//Check Parse one string with unicode chars
	{
		std::string jsonString = "{\"str\":\"Le\\u00F1a:\\u00E1\\u00E9\\u00ED\\u00F3\\u00FA\"}";
		std::string utf8String = "\x4C\x65\xC3\xB1\x61\x3a\xC3\xA1\xC3\xA9\xC3\xAD\xC3\xB3\xC3\xBA";

		JSONObject jobj;
		EXPECT_TRUE (JSON_ERR_CODE::SUCCESS == JSONParser::parse (jobj, jsonString.c_str ()));

		PtrJSONBase base = jobj.get ("str");

		bool isString = (base->getType () == JSON_TYPE::JSTRING);
		EXPECT_TRUE (isString);
		if (isString)
		{
			std::string parsed (base->getAsString ());
			EXPECT_TRUE (parsed.compare (utf8String) == 0);
		}
	}
}
