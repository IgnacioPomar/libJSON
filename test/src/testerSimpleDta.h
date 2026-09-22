/*********************************************************************************************
*	Name		: testerSimpleDta.h
*	Description	: BAsic data for the tests
********************************************************************************************/


#pragma once

#include "libJSON.h"


constexpr int NUM_ITEMS_IN_SIMPLE_DTA_ARRAY = 5;

constexpr const char* jsonTxt = "{\"arr\":[{\"lid\":0,\"x2\":0},{\"lid\":1,\"x2\":2},{\"lid\":2,\"x2\":4},{\"lid\":3,\"x2\":6},{\"lid\":4,\"x2\":8}]}";


// Checks the {"arr":[{"lid":i,"x2":2*i}, ...]} structure by value instead of comparing a
// serialized string: JSONObject stores its keys in an unordered_map, so the key order produced
// by toString() is implementation-defined (it can vary across compilers/STL versions) and must
// not be relied upon.
inline bool checkSimpleDtaArray (JSONObject& jobj)
{
	PtrJSONBase base = jobj.get ("arr");
	if (!base || base->getType () != JSON_TYPE::JARR)
	{
		return false;
	}

	JSONArray arr = base->getAsArray ();
	int count = 0;
	for (auto& elem : arr)
	{
		if (elem->getType () != JSON_TYPE::JOBJ)
		{
			return false;
		}

		JSONObject child = elem->getAsObject ();
		if (child.getInt ("lid") != count || child.getInt ("x2") != count * 2)
		{
			return false;
		}
		count++;
	}

	return count == NUM_ITEMS_IN_SIMPLE_DTA_ARRAY;
}
