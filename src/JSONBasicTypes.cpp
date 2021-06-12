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


JSONDouble::JSONDouble (double value) : value (value) {}

std::string JSONDouble::toString () const
{
	return  std::to_string (value);
}


JSONString::JSONString (std::string &value) : value (value) {}

std::string JSONString::toString () const
{
	return "\"" + value + "\"";
}


std::string JSONNull::toString () const
{
	return std::string ("null");
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
