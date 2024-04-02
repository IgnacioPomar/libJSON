/*********************************************************************************************
*	Name        : 002_parseJSON.cpp
*  Description : Test creating JSON from vars
********************************************************************************************/

#include <set>
#include <string>
#include <fstream>
#include <streambuf>
#include <TinyCppUnit/TinyCppUnit.h>

#include "libJSON.h"

#include "testerSimpleDta.h"


constexpr int NUM_OBJECTS_IN_PLAIN_DTA_ARRAY = 5;


UNIT_TEST_CASE (TestIterateJSONArray)
{

	JSONObject jobj;

	JSONParser::parseFromFile (jobj, "../test/data/simpleDtaWithSpaces.json");


	//Iterate througt the array
	PtrJSONBase base = jobj.get ("arr");

	if (UNIT_CHECK (base->getType () == JSON_TYPE::JARR))
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
		UNIT_CHECK (workedAsExpected);
		UNIT_CHECK (count == NUM_OBJECTS_IN_PLAIN_DTA_ARRAY);
	}
}



UNIT_TEST_CASE (TestIterateJSONObject)
{

	JSONObject jobj;

	JSONParser::parseFromFile (jobj, "../test/data/ObjectSerie.json");

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
	UNIT_CHECK (workedAsExpected);
	UNIT_CHECK (count == NUM_OBJECTS_IN_PLAIN_DTA_ARRAY);

}


