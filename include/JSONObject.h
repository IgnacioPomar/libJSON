/*********************************************************************************************
*	Name		: JSONObject.h
*	Description	: Keeps a named array
********************************************************************************************/


#pragma once
#ifndef _JSON_OBJECT_
#define _JSON_OBJECT_

#include <unordered_map>

#include "libJSON_cfg.h"
#include "libJSON.h"


typedef std::unordered_map<std::string, PtrJSONBase> ObjContainer;

class  JSONObject : public JSONBase
{
private:
	std::shared_ptr<ObjContainer> container;
public:
	LIBJSON_API JSONObject (const JSONObject& other);
	LIBJSON_API JSONObject ();

	LIBJSON_API std::string toString () const;

	LIBJSON_API void put (const char * key, int value);
	LIBJSON_API void put (const char * key, double value);
	LIBJSON_API void put (const char* key, const char * value);
	LIBJSON_API void put (const char* key, JSONArray& arr);
	LIBJSON_API void put (const char * key, JSONObject& obj);

};

#endif //_JSON_OBJECT_
