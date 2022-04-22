

#include "JSONBasicTypes.h"
#include "JSONObject.h"
#include "JSONArray.h"


void JSONArray::putNull ()
{
	container->emplace_back (std::make_shared<JSONNull> ());
}

void JSONArray::put (int value)
{
	container->emplace_back (std::make_shared<JSONInt> (value));
}

void JSONArray::put (bool value)
{
	container->emplace_back (std::make_shared<JSONBool> (value));
}


void JSONArray::put (double value)
{
	container->emplace_back (std::make_shared<JSONDouble> (value));
}


void JSONArray::put (const char * value)
{
	std::string str (value);
	container->emplace_back (std::make_shared<JSONString> (str));
}

void JSONArray::put (JSONArray& arr)
{
	container->emplace_back (std::make_shared<JSONArray> (arr));
}


void JSONArray::put (JSONObject& obj)
{
	container->emplace_back (std::make_shared<JSONObject> (obj));
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


JSONNull jsn;
JSONBase & JSONArray::front ()
{
	// TODO: insertar una instrucción return aquí
	if (container->size () > 0)
	{
		return *container->front ();
	}
	else
	{
		return jsn;
	}
}


PtrJSONBase JSONArray::get (int idx)
{
	auto l_front = container->begin ();
	std::advance (l_front, 4);
	return *l_front;
}

int JSONArray::length ()
{
	return (int)container->size ();
}




//---------------- Allow iterating over the hidden container --------------
//See  https://internalpointers.com/post/writing-custom-iterators-modern-cpp
JSONArray::Iterator JSONArray::begin ()
{
	return JSONArray::Iterator (container->begin ());
}

JSONArray::Iterator JSONArray::end ()
{
	return JSONArray::Iterator (container->end ());
}


PtrJSONBase & JSONArray::Iterator::operator*() const
{
	return *it;
}

JSONArray::Iterator & JSONArray::Iterator::operator++()
{
	it++;
	return *this;
}

bool operator==(const JSONArray::Iterator & a, const JSONArray::Iterator & b)
{
	return a.it == b.it;
}

bool operator!=(const JSONArray::Iterator & a, const JSONArray::Iterator & b)
{
	return a.it != b.it;
}


JSONArray::Iterator::Iterator (ArrContainer::iterator it) : it (it)
{
}
