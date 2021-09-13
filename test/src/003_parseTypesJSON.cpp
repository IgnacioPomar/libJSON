/*********************************************************************************************
*	Name        : 003_perseTypesJSON.cpp
*  Description : Test parsing all types of the JSON
********************************************************************************************/

#include <string>
#include <fstream>
#include <streambuf>
#include <TinyCppUnit/TinyCppUnit.h>

#include "libJSON.h"
#include "JSONParseUchar.h"



UNIT_TEST_CASE (TestJSONTypes)
{
	JSONObject jobj;

	std::ifstream ifs ("../test/data/withAllDataTypes.json", std::ios::in);
	std::string str (std::istreambuf_iterator<char>{ifs}, {});

	UNIT_REQUIRE (JSON_ERR_CODE::SUCCESS == JSONParser::parse (jobj, str.c_str ()));


	//Existing fields
	UNIT_CHECK (jobj.get ("id")->getType () == JSON_TYPE::JSTRING);
	UNIT_CHECK (jobj.get ("name")->getType () == JSON_TYPE::JSTRING);
	UNIT_CHECK (jobj.get ("isYummy")->getType () == JSON_TYPE::JBOOL);
	UNIT_CHECK (jobj.get ("stock")->getType () == JSON_TYPE::JINT);
	UNIT_CHECK (jobj.get ("price")->getType () == JSON_TYPE::JDOUBLE);
	UNIT_CHECK (jobj.get ("lastOvenError")->getType () == JSON_TYPE::JNULL);
	UNIT_CHECK (jobj.get ("images")->getType () == JSON_TYPE::JARR);
	UNIT_CHECK (jobj.get ("visual")->getType () == JSON_TYPE::JOBJ);

	//Not existing fiels
	UNIT_CHECK (jobj.get ("noExists")->getType () == JSON_TYPE::JNULL);

}




UNIT_TEST_CASE (TestEmbeddedUnicode)
{
	//Check Parse one unicode char
	{
		std::string ucharStr = "00F1";
		std::string utf8Str = "\xC3\xB1";

		std::string parsed = JSONParseUchar::getUTF8Char (ucharStr);

		UNIT_CHECK (parsed.compare (utf8Str) == 0);
	}


	//Check Parse one string with unicode chars
	{
		std::string jsonString = "{\"str\":\"Le\\u00F1a:\\u00E1\\u00E9\\u00ED\\u00F3\\u00FA\"}";
		std::string utf8String = "\x4C\x65\xC3\xB1\x61\x3a\xC3\xA1\xC3\xA9\xC3\xAD\xC3\xB3\xC3\xBA";

		JSONObject jobj;
		UNIT_CHECK (JSON_ERR_CODE::SUCCESS == JSONParser::parse (jobj, jsonString.c_str ()));

		PtrJSONBase base = jobj.get ("str");

		if (UNIT_CHECK (base->getType () == JSON_TYPE::JSTRING))
		{
			std::string parsed (base->getAsString ());
			UNIT_CHECK (parsed.compare (utf8String) == 0);
		}
	}
}
