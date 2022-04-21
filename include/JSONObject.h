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
public:
	LIBJSON_API JSONObject (const JSONObject& other);
	LIBJSON_API JSONObject ();

	LIBJSON_API std::string toString () const;
	LIBJSON_API JSON_TYPE getType () const;

	LIBJSON_API void putNull (const char * key);
	LIBJSON_API void put (const char * key, bool value);
	LIBJSON_API void put (const char * key, int value);
	LIBJSON_API void put (const char * key, double value);
	LIBJSON_API void put (const char* key, const char * value);
	LIBJSON_API void put (const char* key, JSONArray& arr);
	LIBJSON_API void put (const char * key, JSONObject& obj);

	LIBJSON_API PtrJSONBase get (const char * key);

	LIBJSON_API bool getBool (const char * key);
	LIBJSON_API int getInt (const char * key);
	LIBJSON_API double getDouble (const char * key);
	LIBJSON_API const char * getString (const char * key);
	LIBJSON_API JSONArray getArray (const char * key);
	LIBJSON_API JSONObject getObject (const char * key);

};

#endif //_JSON_OBJECT_
