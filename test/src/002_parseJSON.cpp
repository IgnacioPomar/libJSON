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

		std::ifstream ifs ("../test/data/simpleDtaWithSpaces.json", std::ios::in);
		std::string str (std::istreambuf_iterator<char>{ifs}, {});

		UNIT_CHECK (JSON_ERR_CODE::SUCCESS == JSONParser::parse (jobj, str.c_str()));
		UNIT_CHECK (0 == jobj.toString ().compare (jsonTxt));
	}

}
