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

		std::ifstream ifs ("../test/data/simpleDtaWithSpaces.json", std::ios::in);
		std::string str (std::istreambuf_iterator<char>{ifs}, {});

		UNIT_CHECK (JSON_ERR_CODE::SUCCESS == JSONParser::parse (jobj, str.c_str ()));
		UNIT_CHECK (0 == jobj.toString ().compare (jsonTxt));

		//Iterate througt the array
		PtrJSONBase base = jobj.get ("arr");

		if (UNIT_CHECK (base->getType () == JSON_TYPE::JARR))
		{
			int count = 0;
			bool workedAsExpected = true;

			JSONArray arr = base->getAsArray ();
			for (auto objElem : arr)
			{
				if (objElem->getType () == JSON_TYPE::JOBJ)
				{
					int lid = objElem->getAsObject ().get ("lid")->getAsInt ();
					workedAsExpected = workedAsExpected && lid == count;
					count++;
				}
				else
				{
					workedAsExpected = false;
				}

			}
			UNIT_CHECK (workedAsExpected);
			UNIT_CHECK (count == NUM_OBJECTS_IN_PLAIN_DTA_ARRAY);
		}
	}
}
