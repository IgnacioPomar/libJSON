/*********************************************************************************************
*	Name        : 001_CreateJSON.cpp
*  Description : Test creating JSON from vars
********************************************************************************************/

#include <TinyCppUnit/TinyCppUnit.h>

#include "libJSON.h"

#include "testerSimpleDta.h"



void fillObjWithSimpleDta (JSONObject* jobj)
{
	JSONArray jarr;
	for (int i = 0; i < 5; i++)
	{
		JSONObject jObjChild;
		jObjChild.put ("lid", i);
		jObjChild.put ("x2", i * 2);


		jarr.put (jObjChild);
	}

	jobj->put ("arr", jarr);

}


UNIT_TEST_CASE(TestCreateJSON)
{
	JSONObject jobj;
	fillObjWithSimpleDta (&jobj);
	UNIT_CHECK (0 == jobj.toString ().compare (jsonTxt));
}
