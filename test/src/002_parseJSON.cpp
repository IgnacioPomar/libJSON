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
		//std::ifstream ifs ("simpleObject.json", std::ios::in);
		//std::string str (std::istreambuf_iterator<char>{ifs}, {});

		const char* jsonTxt = "{\"arr\":[{\"lid\":0,\"x2\":0},{\"lid\":1,\"x2\":2},{\"lid\":2,\"x2\":4},{\"lid\":3,\"x2\":6},{\"lid\":4,\"x2\":8}]}";

		JSONObject jobj;

		JSON_ERR_CODE jsonErr = JSONParser::parse (jobj, jsonTxt);

		UNIT_CHECK (jsonErr == JSON_ERR_CODE::SUCCESS);


		printf (jobj.toString ().c_str ());



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
