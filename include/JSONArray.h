/*********************************************************************************************
*	Name		: JSONArray.h
*	Description	: Keeps the array Values
********************************************************************************************/


#pragma once
#ifndef _JSON_ARRAY_
#define _JSON_ARRAY_

#include "libJSON_cfg.h"
#include "libJSON.h"


class  JSONArray : public JSONBase
{
private:
	std::shared_ptr<ArrContainer> container;
public:
	LIBJSON_API JSONArray (const JSONArray& other);
	LIBJSON_API JSONArray ();

	LIBJSON_API std::string toString () const;

	LIBJSON_API void put (int value);
	LIBJSON_API void put (double value);
	LIBJSON_API void put (const char * value);
	LIBJSON_API void put (JSONArray& arr);
	LIBJSON_API void put (JSONObject& obj);

};



#endif //_JSON_ARRAY_
