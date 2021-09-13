#include "JSONBasicTypes.h"
#include "JSONObject.h"
#include "JSONArray.h"

void JSONObject::put (const char* key, const char * value)
{
	std::string str (value);
	container->emplace (key, std::make_shared<JSONString> (str));
}



void JSONObject::putNull (const char* key)
{
	container->emplace (key, std::make_shared<JSONNull> ());
}

void JSONObject::put (const char* key, bool value)
{
	container->emplace (key, std::make_shared<JSONBool> (value));
}

void JSONObject::put (const char* key, int value)
{
	container->emplace (key, std::make_shared<JSONInt> (value));
}

void JSONObject::put (const char * key, double value)
{
	container->emplace (key, std::make_shared<JSONDouble> (value));
}

void JSONObject::put (const char* key, JSONArray& arr)
{
	container->emplace (key, std::make_shared<JSONArray> (arr));
}

void JSONObject::put (const char* key, JSONObject& obj)
{
	container->emplace (key, std::make_shared<JSONObject> (obj));
}



PtrJSONBase JSONObject::get (const char * key)
{
	auto search = container->find (key);
	if (search != container->end ())
	{
		return search->second;
	}
	else
	{
		return std::make_shared<JSONNull> ();
	}
}


JSONObject::JSONObject ()
{
	this->container = std::make_shared<ObjContainer> ();
}

JSONObject::JSONObject (const JSONObject& other)
{
	this->container = other.container;
}






std::string JSONObject::toString () const
{
	std::string retVal = "{";
	std::string sep = "";
	for (const auto& kv : *container)
	{
		retVal.append (sep).append ("\"" + kv.first + "\":").append (kv.second->toString ());
		sep = ",";
	}
	retVal.append ("}");
	return retVal;
}



JSON_TYPE JSONObject::getType () const
{
	return JSON_TYPE::JOBJ;
}
