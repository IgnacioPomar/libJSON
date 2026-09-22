/*********************************************************************************************
*	Name        : 002_parseJSON.cpp
*  Description : Test creating JSON from vars
********************************************************************************************/

#include <string>
#include <fstream>
#include <streambuf>
#include <gtest/gtest.h>

#include "libJSON.h"

#include "testerSimpleDta.h"


constexpr int NUM_OBJECTS_IN_PLAIN_DTA_ARRAY = 5;

static const std::string kTestDataDir = TEST_DATA_DIR;


TEST (LibJSON, TestLoadJSON)
{
	//Parse string and check the results
	{
		JSONObject jobj;


		EXPECT_TRUE (JSON_ERR_CODE::SUCCESS == JSONParser::parse (jobj, jsonTxt));
		EXPECT_TRUE (0 == jobj.toString ().compare (jsonTxt));
	}


	//Load the same strings but with spaces
	{
		JSONObject jobj;

		EXPECT_TRUE (JSON_ERR_CODE::SUCCESS == JSONParser::parseFromFile (jobj, (kTestDataDir + "/simpleDtaWithSpaces.json").c_str ()));
		EXPECT_TRUE (0 == jobj.toString ().compare (jsonTxt));
	}
}


TEST (LibJSON, TestLoadErrors)
{
	/*
	Outside the dll, is not possible to fail in this case
	JSONBool jbool;
	EXPECT_TRUE (JSON_ERR_CODE::NOT_CONTAINER_TYPE == JSONParser::parse (jbool, jsonTxt));
	*/


	JSONObject jobj;
	JSONArray jarr;

	//Wrong file
	EXPECT_TRUE (JSON_ERR_CODE::FILE_ACCESS_ERROR == JSONParser::parseFromFile (jobj, "wrongFile.json"));

	//wrong format
	EXPECT_TRUE (JSON_ERR_CODE::NO_MATCHING_OBJECT == JSONParser::parse (jobj, "[\"abc\",123]"));
	EXPECT_TRUE (JSON_ERR_CODE::NO_MATCHING_OBJECT == JSONParser::parse (jarr, "{\"abc\":123}"));


	EXPECT_TRUE (JSON_ERR_CODE::WRONG_STRING_FORMAT == JSONParser::parse (jobj, "{\"abc:123}"));
	EXPECT_TRUE (JSON_ERR_CODE::WRONG_STRING_FORMAT == JSONParser::parse (jobj, "{\"abc\":\"123}"));
	EXPECT_TRUE (JSON_ERR_CODE::WRONG_STRING_FORMAT == JSONParser::parse (jobj, "{\"abc\":\"\\z123\"}"));
	EXPECT_TRUE (JSON_ERR_CODE::WRONG_STRING_FORMAT == JSONParser::parse (jobj, "{\"abc\":\"\\uZ123\"}"));

	EXPECT_TRUE (JSON_ERR_CODE::WRONG_NUMBER_FORMAT == JSONParser::parse (jobj, "{\"abc\":z123}"));
	EXPECT_TRUE (JSON_ERR_CODE::WRONG_NUMBER_FORMAT == JSONParser::parse (jobj, "{\"abc\":123.}"));
	EXPECT_TRUE (JSON_ERR_CODE::WRONG_NUMBER_FORMAT == JSONParser::parse (jobj, "{\"abc\":123.1EE}"));

	EXPECT_TRUE (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jobj, "{\"abc\":12z3}"));
	EXPECT_TRUE (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jobj, "{\"abc\":troe}"));
	EXPECT_TRUE (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jobj, "{\"abc\":falso}"));
	EXPECT_TRUE (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jobj, "{\"abc\":nil}"));

	EXPECT_TRUE (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jarr, "[\"abc\":123]"));
	EXPECT_TRUE (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jobj, "{\"abc\",123}"));
	EXPECT_TRUE (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jarr, "[\"abc\",123}"));
	EXPECT_TRUE (JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE == JSONParser::parse (jobj, "{\"abc\",123]"));

	EXPECT_TRUE (JSON_ERR_CODE::UNEXPECTED_END_OF_STRING == JSONParser::parse (jobj, "{\"abc\":123"));
	EXPECT_TRUE (JSON_ERR_CODE::UNEXPECTED_END_OF_STRING == JSONParser::parse (jarr, "[\"abc\",123"));



}
