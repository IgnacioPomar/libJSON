/*********************************************************************************************
*	Name        : 002_parseJSON.cpp
*  Description : Test creating JSON from vars
********************************************************************************************/

#include <string>
#include <fstream>
#include <streambuf>
#include <TinyCppUnit/TinyCppUnit.h>

#include "libJSON.h"

#include "testerSimpleDta.h"


constexpr int NUM_OBJECTS_IN_PLAIN_DTA_ARRAY = 5;


UNIT_TEST_CASE (TestLoadJSON)
{
	//Parse string and check the results
	{
		JSONObject jobj;


		UNIT_CHECK (JSON_ERR_CODE::SUCCESS == JSONParser::parse (jobj, jsonTxt));
		UNIT_CHECK (0 == jobj.toString ().compare (jsonTxt));
	}


	//Load the same strings but with spaces
	{
		JSONObject jobj;

		UNIT_CHECK (JSON_ERR_CODE::SUCCESS == JSONParser::parseFromFile (jobj, "../test/data/simpleDtaWithSpaces.json"));
		UNIT_CHECK (0 == jobj.toString ().compare (jsonTxt));
	}
}


UNIT_TEST_CASE (TestLoadErrors)
{
	/*
	Outside the dll, is not possible to fail in this case
	JSONBool jbool;
	UNIT_CHECK (JSON_ERR_CODE::NOT_CONTAINER_TYPE == JSONParser::parse (jbool, jsonTxt));
	*/


	JSONObject jobj;
	JSONArray jarr;

	//Wrong file
	UNIT_CHECK (JSON_ERR_CODE::FILE_ACCESS_ERROR == JSONParser::parseFromFile (jobj, "wrongFile.json"));

	//wrong format
	UNIT_CHECK (JSON_ERR_CODE::NO_MATCHING_OBJECT == JSONParser::parse (jobj, "[\"abc\",123]"));
	UNIT_CHECK (JSON_ERR_CODE::NO_MATCHING_OBJECT == JSONParser::parse (jarr, "{\"abc\":123}"));


	UNIT_CHECK (JSON_ERR_CODE::WRONG_STRING_FORMAT == JSONParser::parse (jobj, "{\"abc:123}"));
	UNIT_CHECK (JSON_ERR_CODE::WRONG_STRING_FORMAT == JSONParser::parse (jobj, "{\"abc\":\"123}"));
	UNIT_CHECK (JSON_ERR_CODE::WRONG_STRING_FORMAT == JSONParser::parse (jobj, "{\"abc\":\"\\z123\"}"));
	UNIT_CHECK (JSON_ERR_CODE::WRONG_STRING_FORMAT == JSONParser::parse (jobj, "{\"abc\":\"\\uZ123\"}"));

	UNIT_CHECK (JSON_ERR_CODE::WRONG_NUMBER_FORMAT == JSONParser::parse (jobj, "{\"abc\":z123}"));
	UNIT_CHECK (JSON_ERR_CODE::WRONG_NUMBER_FORMAT == JSONParser::parse (jobj, "{\"abc\":123.}"));
	UNIT_CHECK (JSON_ERR_CODE::WRONG_NUMBER_FORMAT == JSONParser::parse (jobj, "{\"abc\":123.1EE}"));

	UNIT_CHECK (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jobj, "{\"abc\":12z3}"));
	UNIT_CHECK (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jobj, "{\"abc\":troe}"));
	UNIT_CHECK (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jobj, "{\"abc\":falso}"));
	UNIT_CHECK (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jobj, "{\"abc\":nil}"));

	UNIT_CHECK (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jarr, "[\"abc\":123]"));
	UNIT_CHECK (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jobj, "{\"abc\",123}"));
	UNIT_CHECK (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jarr, "[\"abc\",123}"));
	UNIT_CHECK (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jobj, "{\"abc\",123]"));

	UNIT_CHECK (JSON_ERR_CODE::UNEXPECTED_END_OF_STRING == JSONParser::parse (jobj, "{\"abc\":123"));
	UNIT_CHECK (JSON_ERR_CODE::UNEXPECTED_END_OF_STRING == JSONParser::parse (jarr, "[\"abc\",123"));



}
