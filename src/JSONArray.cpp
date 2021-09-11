﻿

#include "JSONBasicTypes.h"
#include "JSONObject.h"
#include "JSONArray.h"

void JSONArray::put (int value)
{
	container->emplace_back (std::make_unique<JSONInt> (value));
}

LIBJSON_API void JSONArray::put (double value)
{
	container->emplace_back (std::make_unique<JSONDouble> (value));
}


void JSONArray::put (const char * value)
{
	std::string str (value);
	container->emplace_back (std::make_unique<JSONString> (str));
}

void JSONArray::put (JSONArray& arr)
{
	container->emplace_back (std::make_unique<JSONArray> (arr));
}


void JSONArray::put (JSONObject& obj)
{
	container->emplace_back (std::make_unique<JSONObject> (obj));
}



std::string JSONArray::toString () const
{
	std::string retVal = "[";
	std::string sep = "";
	for (const auto& kv : *container)
	{
		retVal.append (sep).append (kv->toString ());
		sep = ",";
	}
	retVal.append ("]");
	return retVal;
}


JSON_TYPE JSONArray::getType () const
{
	return JSON_TYPE::JARR;
}




JSONArray::JSONArray ()
{
	this->container = std::make_shared<ArrContainer> ();
}

JSONArray::JSONArray (const JSONArray& other)
{
	this->container = other.container;
}
