/*********************************************************************************************
*	Name        : 001_CreateJSON.cpp
*  Description : Test creating JSON from vars
********************************************************************************************/

#include <gtest/gtest.h>

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


TEST (LibJSON, TestCreateJSON)
{
	JSONObject jobj;
	fillObjWithSimpleDta (&jobj);
	EXPECT_TRUE (0 == jobj.toString ().compare (jsonTxt));
}
