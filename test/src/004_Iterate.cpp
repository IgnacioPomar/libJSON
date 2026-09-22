/*********************************************************************************************
*	Name        : 002_parseJSON.cpp
*  Description : Test creating JSON from vars
********************************************************************************************/

#include <set>
#include <string>
#include <fstream>
#include <streambuf>
#include <gtest/gtest.h>

#include "libJSON.h"

#include "testerSimpleDta.h"


constexpr int NUM_OBJECTS_IN_PLAIN_DTA_ARRAY = 5;

static const std::string kTestDataDir = TEST_DATA_DIR;


TEST (LibJSON, TestIterateJSONArray)
{

	JSONObject jobj;

	JSONParser::parseFromFile (jobj, (kTestDataDir + "/simpleDtaWithSpaces.json").c_str ());


	//Iterate througt the array
	PtrJSONBase base = jobj.get ("arr");

	bool isArray = (base->getType () == JSON_TYPE::JARR);
	EXPECT_TRUE (isArray);
	if (isArray)
	{
		int count = 0;
		bool workedAsExpected = true;

		JSONArray arr = base->getAsArray ();
		for (auto& objElem : arr)
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
		EXPECT_TRUE (workedAsExpected);
		EXPECT_TRUE (count == NUM_OBJECTS_IN_PLAIN_DTA_ARRAY);
	}
}



TEST (LibJSON, TestIterateJSONObject)
{

	JSONObject jobj;

	JSONParser::parseFromFile (jobj, (kTestDataDir + "/ObjectSerie.json").c_str ());

	int count = 0;
	bool workedAsExpected = true;

	std::set<std::string> expectedKeys = {"obj1", "obj2", "obj3", "4", "z"};

	for (auto& objElem : jobj)
	{
		if (expectedKeys.find (objElem.key ()) == expectedKeys.end ())
		{
			workedAsExpected = false;
		}
		count++;
	}
	EXPECT_TRUE (workedAsExpected);
	EXPECT_TRUE (count == NUM_OBJECTS_IN_PLAIN_DTA_ARRAY);

}
