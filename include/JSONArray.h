/*********************************************************************************************
*	Name		: JSONArray.h
*	Description	: Keeps the array Values
********************************************************************************************/


#pragma once
#ifndef _JSON_ARRAY_
#define _JSON_ARRAY_
#include <list>
#include <iterator>

#include "libJSON_cfg.h"
#include "JSONBase.h"


typedef std::list<PtrJSONBase> ArrContainer;

class  JSONArray : public JSONBase
{
private:
	std::shared_ptr<ArrContainer> container;
public:
	LIBJSON_API JSONArray (const JSONArray& other);
	LIBJSON_API JSONArray ();

	LIBJSON_API std::string toString () const;
	LIBJSON_API JSON_TYPE getType () const;

	LIBJSON_API void putNull ();
	LIBJSON_API void put (bool value);
	LIBJSON_API void put (int value);
	LIBJSON_API void put (double value);
	LIBJSON_API void put (const char * value);
	LIBJSON_API void put (JSONArray& arr);
	LIBJSON_API void put (JSONObject& obj);

	//LIBJSON_API PtrJSONBase get (int idx);

	//---------------- Para Iterar por la clase --------------
	struct Iterator
	{
		Iterator (ArrContainer::iterator it);

		LIBJSON_API PtrJSONBase & operator*() const;
		//PtrJSONBase * operator->();
		LIBJSON_API Iterator& operator++();
		//Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
		LIBJSON_API friend bool operator== (const Iterator& a, const Iterator& b);
		LIBJSON_API friend bool operator!= (const Iterator& a, const Iterator& b);

	private:
		ArrContainer::iterator it;
	};

	LIBJSON_API  Iterator begin ();
	LIBJSON_API  Iterator end ();

};



#endif //_JSON_ARRAY_
