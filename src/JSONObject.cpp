#include "JSONBasicTypes.h"
#include "JSONObject.h"
#include "JSONArray.h"

namespace
{
	// Matches the previous std::unordered_map::emplace() semantics: if the key is
	// already present, the new value is discarded and the existing one is kept.
	void emplaceIfAbsent (ObjContainer& container, const char* key, PtrJSONBase value)
	{
		for (const auto& kv : container)
		{
			if (kv.first == key) return;
		}
		container.emplace_back (key, std::move (value));
	}
}

void JSONObject::put (const char* key, const char* value)
{
	std::string str (value);
	emplaceIfAbsent (*container, key, std::make_shared<JSONString> (str));
}



void JSONObject::putNull (const char* key)
{
	emplaceIfAbsent (*container, key, std::make_shared<JSONNull> ());
}

void JSONObject::put (const char* key, bool value)
{
	emplaceIfAbsent (*container, key, std::make_shared<JSONBool> (value));
}

void JSONObject::put (const char* key, int value)
{
	emplaceIfAbsent (*container, key, std::make_shared<JSONInt> (value));
}

void JSONObject::put (const char* key, double value)
{
	emplaceIfAbsent (*container, key, std::make_shared<JSONDouble> (value));
}

void JSONObject::put (const char* key, JSONArray& arr)
{
	emplaceIfAbsent (*container, key, std::make_shared<JSONArray> (arr));
}

void JSONObject::put (const char* key, JSONObject& obj)
{
	emplaceIfAbsent (*container, key, std::make_shared<JSONObject> (obj));
}



PtrJSONBase JSONObject::get (const char* key)
{
	for (const auto& kv : *container)
	{
		if (kv.first == key) return kv.second;
	}
	return std::make_shared<JSONNull> ();
}

bool JSONObject::getBool (const char* key)
{
	return this->get (key)->getAsBool ();
}
int JSONObject::getInt (const char* key)
{
	return this->get (key)->getAsInt ();
}
double JSONObject::getDouble (const char* key)
{
	return this->get (key)->getAsDouble ();
}
const char* JSONObject::getString (const char* key)
{
	return this->get (key)->getAsString ();
}

JSONArray JSONObject::getArray (const char* key)
{
	return this->get (key)->getAsArray ();
}

JSONObject JSONObject::getObject (const char* key)
{
	return this->get (key)->getAsObject ();
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
	std::string retVal;
	appendTo (retVal);
	return retVal;
}

void JSONObject::appendTo (std::string& out) const
{
	out += '{';
	bool first = true;
	for (const auto& kv : *container)
	{
		if (!first) out += ',';
		first = false;

		out += '"';
		out += kv.first;
		out += "\":";
		kv.second->appendTo (out);
	}
	out += '}';
}



JSON_TYPE JSONObject::getType () const
{
	return JSON_TYPE::JOBJ;
}




//---------------- Allow iterating over the hidden container --------------
//See  https://internalpointers.com/post/writing-custom-iterators-modern-cpp
JSONObject::Iterator JSONObject::begin ()
{
	return JSONObject::Iterator (container->begin ());
}

JSONObject::Iterator JSONObject::end ()
{
	return JSONObject::Iterator (container->end ());
}


JSONObject::Element& JSONObject::Iterator::operator*()
{
	return elem;
}

JSONObject::Iterator& JSONObject::Iterator::operator++()
{
	elem.it++;
	return *this;
}


bool operator==(const JSONObject::Iterator& a, const JSONObject::Iterator& b)
{
	return a.elem.it == b.elem.it;
}

bool operator!=(const JSONObject::Iterator& a, const JSONObject::Iterator& b)
{
	return a.elem.it != b.elem.it;
}


JSONObject::Iterator::Iterator (ObjContainer::iterator it) : elem (it)
{
}

JSONObject::Element::Element (ObjContainer::iterator it) : it (it)
{
}

std::string JSONObject::Element::key () const
{
	return it->first;
}

PtrJSONBase& JSONObject::Element::value () const
{
	return it->second;
}

