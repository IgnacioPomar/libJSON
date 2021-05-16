/*********************************************************************************************
*	Name        : 001_CreateJSON.cpp
*  Description : Test creating JSON from vars
********************************************************************************************/

#include <TinyCppUnit/TinyCppUnit.h>

#include "libJSON.h"



UNIT_TEST_CASE(TestCreateJSON)
{
	JSONObject jobj;

	JSONArray jarr;
	for (int i = 0; i < 5; i++)
	{
		JSONObject jObjChild;
		jObjChild.put("lid", i);
		jObjChild.put("x2", i * 2);


		jarr.put(jObjChild);
	}

	jobj.put("arr", jarr);

	printf(jobj.toString().c_str());
}
