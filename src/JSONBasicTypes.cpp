/*********************************************************************************************
*	Name		: JSONBasicTypes.cpp
*	Description	: Keeps a named array
********************************************************************************************/

#include "JSONBasicTypes.h"


JSONInt::JSONInt (int value) : value (value) {}

std::string JSONInt::toString () const
{
	return  std::to_string (value);
}

JSON_TYPE JSONInt::getType () const
{
	return JSON_TYPE::JINT;
}


JSONDouble::JSONDouble (double value) : value (value) {}

std::string JSONDouble::toString () const
{
	return  std::to_string (value);
}

JSON_TYPE JSONDouble::getType () const
{
	return JSON_TYPE::JDOUBLE;
}


JSONString::JSONString (std::string &value) : value (value) {}

std::string JSONString::toString () const
{
	return "\"" + value + "\"";
}

JSON_TYPE JSONString::getType () const
{
	return JSON_TYPE::JSTRING;
}


std::string JSONNull::toString () const
{
	return std::string ("null");
}

JSON_TYPE JSONNull::getType () const
{
	return JSON_TYPE::JNULL;
}


std::string JSONBool::toString () const
{
	const char* ret = (value) ? "true" : "false";
	return std::string (ret);
}

JSONBool::JSONBool (bool value)
{
	this->value = value;
}

JSON_TYPE JSONBool::getType () const
{
	return JSON_TYPE::JBOOL;
}
