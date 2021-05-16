/*********************************************************************************************
*	Name        : 001_CreateJSON.cpp
*  Description : Test creating JSON from vars
********************************************************************************************/

#include <unordered_map>
#include <forward_list>
#include <string>
#include <memory>
#include <TinyCppUnit/TinyCppUnit.h>

//#include "libJSON.h"


class  JSONBase
{
public:
	virtual std::string toString () const = 0;

};

typedef std::unique_ptr<JSONBase> PtrJSONBase;
typedef std::forward_list<PtrJSONBase> ArrContainer;
typedef std::unordered_map<std::string, PtrJSONBase> ObjContainer;




class  JSONObject;


class  JSONArray : public JSONBase
{
private:
	std::shared_ptr<ArrContainer> container;
public:
	JSONArray (const JSONArray& other);
	JSONArray ();

	std::string toString () const;

	void put (int value);
	void put (std::string value);
	void put (JSONArray & arr);
	void put (JSONObject & obj);

};


class  JSONObject : public JSONBase
{
private:
	std::shared_ptr<ObjContainer> container;
public:
	JSONObject (const JSONObject& other);
	JSONObject ();

	std::string toString () const;

	void put (std::string key, int value);
	void put (std::string key, std::string value);
	void put (std::string key, JSONArray & arr);
	void put (std::string key, JSONObject & obj);

};




class  JSONInt : public JSONBase
{
private:
	int value;
public:
	JSONInt (int value);
	std::string toString () const;
};

class  JSONString : public JSONBase
{
private:
	std::string value;
public:
	JSONString (std::string value);
	std::string toString () const;


};

//------------------- Implementacion ----------------------
std::string JSONInt::toString () const
{
	return  std::to_string (value);
}

JSONInt::JSONInt (int value) : value (value) {}


JSONString::JSONString (std::string value) : value (value) {}

std::string JSONString::toString () const
{
	return "\"" + value + "\"";
}


//------ arr -----------
void JSONArray::put (int value)
{
	container->push_front (std::make_unique<JSONInt> (value));
}

void JSONArray::put (std::string value)
{
	container->push_front (std::make_unique<JSONString> (value));
}

void JSONArray::put (JSONArray & arr)
{
	container->emplace_front (std::make_unique<JSONArray> (arr));
}


void JSONArray::put (JSONObject & obj)
{
	container->emplace_front (std::make_unique<JSONObject> (obj));
}



std::string JSONArray::toString () const
{
	std::string retVal = "[";
	std::string sep = "";
	for (const auto&kv : *container)
	{
		retVal.append (sep).append (kv->toString ());
		sep = ",";
	}
	retVal.append ("]");
	return retVal;
}



JSONArray::JSONArray ()
{
	this->container = std::make_shared<ArrContainer> ();
}

JSONArray::JSONArray (const JSONArray & other)
{
	this->container = other.container;
}




//------ obj -----------

void JSONObject::put (std::string key, std::string value)
{
	container->emplace (key, std::make_unique<JSONString> (value));
}


void JSONObject::put (std::string key, int value)
{
	container->emplace (key, std::make_unique<JSONInt> (value));
}

void JSONObject::put (std::string key, JSONArray & arr)
{
	container->emplace (key, std::make_unique<JSONArray> (arr));
}

void JSONObject::put (std::string key, JSONObject & obj)
{
	container->emplace (key, std::make_unique<JSONObject> (obj));
}


JSONObject::JSONObject ()
{
	this->container = std::make_shared<ObjContainer> ();
}

JSONObject::JSONObject (const JSONObject & other)
{
	this->container = other.container;
}






std::string JSONObject::toString () const
{
	std::string retVal = "{";
	std::string sep = "";
	for (const auto&kv : *container)
	{
		retVal.append (sep).append ("\"" + kv.first + "\":").append (kv.second->toString ());
		sep = ",";
	}
	retVal.append ("}");
	return retVal;
}



UNIT_TEST_CASE (TestCreateJSON)
{
	JSONObject jobj;

	JSONArray jarr;
	for (int i = 0; i < 5; i++)
	{
		JSONObject jObjChild;
		jObjChild.put ("lid", i);
		jObjChild.put ("x2", i * 2);


		jarr.put (jObjChild);
	}

	jobj.put ("arr", jarr);

	printf (jobj.toString ().c_str ());
}
