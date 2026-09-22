/*********************************************************************************************
*	Name		: JSONBase.h
*	Description	: Keeps the array Values
********************************************************************************************/


#pragma once
#ifndef _JSON_BASE_
#define _JSON_BASE_


#include <string>
#include <memory>

#include "libJSON_cfg.h"


enum class JSON_TYPE;
class  JSONObject;
class  JSONArray;

class LIBJSON_API JSONBase
{
public:
	virtual JSON_TYPE getType () const = 0;
	virtual std::string toString () const = 0;

	// Appends this value's serialization to an existing buffer instead of returning a new
	// string. JSONObject/JSONArray override this to serialize their whole subtree into the
	// caller's buffer directly, avoiding one extra full-subtree string copy per tree level
	// that toString() would otherwise cause when a container appends a child's returned string.
	virtual void appendTo (std::string& out) const { out += toString (); }

	int getAsInt () const;
	bool getAsBool () const;
	double getAsDouble () const;
	const char * getAsString () const;

	JSONArray getAsArray () const;
	JSONObject getAsObject () const;



};

typedef std::shared_ptr<JSONBase> PtrJSONBase;



enum class JSON_TYPE { JINT, JBOOL, JNULL, JDOUBLE, JSTRING, JOBJ, JARR };
enum class JSON_ERR_CODE { SUCCESS, NO_MATCHING_OBJECT, BAD_FORMAT_UNEXPECTED_VALUE, WRONG_STRING_FORMAT, WRONG_NUMBER_FORMAT, UNEXPECTED_END_OF_STRING, NOT_CONTAINER_TYPE, FILE_ACCESS_ERROR, NOT_IMPLEMENTED };





#endif //_JSON_BASE_
