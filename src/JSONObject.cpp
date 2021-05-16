#include "libJSON.h"

void JSONObject::put(const char* key, const char * value)
{
	std::string str(value);
	container->emplace(key, std::make_unique<JSONString>(str));
}


void JSONObject::put(const char* key, int value)
{
	container->emplace(key, std::make_unique<JSONInt>(value));
}

void JSONObject::put(const char* key, JSONArray& arr)
{
	container->emplace(key, std::make_unique<JSONArray>(arr));
}

void JSONObject::put(const char* key, JSONObject& obj)
{
	container->emplace(key, std::make_unique<JSONObject>(obj));
}


JSONObject::JSONObject()
{
	this->container = std::make_shared<ObjContainer>();
}

JSONObject::JSONObject(const JSONObject& other)
{
	this->container = other.container;
}






std::string JSONObject::toString() const
{
	std::string retVal = "{";
	std::string sep = "";
	for (const auto& kv : *container)
	{
		retVal.append(sep).append("\"" + kv.first + "\":").append(kv.second->toString());
		sep = ",";
	}
	retVal.append("}");
	return retVal;
}

