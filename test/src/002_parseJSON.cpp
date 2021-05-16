/*********************************************************************************************
*	Name        : 002_parseJSON.cpp
*  Description : Test creating JSON from vars
********************************************************************************************/

#include <string>
#include <fstream>
#include <streambuf>
#include <TinyCppUnit/TinyCppUnit.h>

#include "libJSON.h"


UNIT_TEST_CASE (TestLoadJSON)
{
	//Test simple Object
	{
		std::ifstream ifs ("simpleObject.json", std::ios::in);
		std::string str (std::istreambuf_iterator<char>{ifs}, {});



		/*
		JSONObject obj (str.c_str ());
		JSONArray ordenes = obj.getJSONArray ("ord");

	for (int i = 0; i < ordenes.length (); i++)
	{
		JSONObject jsonOrden = ordenes.getJSONObject (i);
		String symbol = jsonOrden.getString ("symbol");
		Integer id = jsonOrden.getInt ("id");
	}

	UNIT_CHECK (props.getInt ("key", 5) == 10);

		*/

	}









}
