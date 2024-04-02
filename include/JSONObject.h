/*********************************************************************************************
*	Name		: JSONObject.h
*	Description	: Keeps a named array
********************************************************************************************/


#pragma once
#ifndef _JSON_OBJECT_
#define _JSON_OBJECT_

#include <unordered_map>

#include "libJSON_cfg.h"
#include "JSONBase.h"


typedef std::unordered_map<std::string, PtrJSONBase> ObjContainer;

class JSONObject : public JSONBase
{
private:
	std::shared_ptr<ObjContainer> container;

	using JSONBase::getAsObject;
	using JSONBase::getAsArray;

	using JSONBase::getAsString;
	using JSONBase::getAsDouble;
	using JSONBase::getAsBool;
	using JSONBase::getAsInt;

public:
	LIBJSON_API JSONObject (const JSONObject& other);
	LIBJSON_API JSONObject ();

	LIBJSON_API std::string toString () const;
	LIBJSON_API JSON_TYPE getType () const;

	LIBJSON_API void putNull (const char* key);
	LIBJSON_API void put (const char* key, bool value);
	LIBJSON_API void put (const char* key, int value);
	LIBJSON_API void put (const char* key, double value);
	LIBJSON_API void put (const char* key, const char* value);
	LIBJSON_API void put (const char* key, JSONArray& arr);
	LIBJSON_API void put (const char* key, JSONObject& obj);

	LIBJSON_API PtrJSONBase get (const char* key);

	LIBJSON_API bool getBool (const char* key);
	LIBJSON_API int getInt (const char* key);
	LIBJSON_API double getDouble (const char* key);
	LIBJSON_API const char* getString (const char* key);
	LIBJSON_API JSONArray getArray (const char* key);
	LIBJSON_API JSONObject getObject (const char* key);

	//---------------- Make it iterable --------------
	class Element
	{
	public:
		Element (ObjContainer::iterator it);

		LIBJSON_API std::string key () const;
		LIBJSON_API PtrJSONBase& value () const;
		//protected:

		ObjContainer::iterator it;
	};

	class Iterator
	{
	public:
		Iterator (ObjContainer::iterator it);

		LIBJSON_API Element& operator*();
		//PtrJSONBase * operator->();
		LIBJSON_API Iterator& operator++();
		//Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
		LIBJSON_API friend bool operator== (const Iterator& a, const Iterator& b);
		LIBJSON_API friend bool operator!= (const Iterator& a, const Iterator& b);



	private:
		Element elem;
		//ObjContainer::iterator it;
	};

	LIBJSON_API  Iterator begin ();
	LIBJSON_API  Iterator end ();


};

#endif //_JSON_OBJECT_
