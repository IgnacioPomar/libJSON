/*********************************************************************************************
*	Name		: JSONBasicTypes.cpp
*	Description	: Keeps a named array
********************************************************************************************/

#include "JSONBasicTypes.h"


JSONInt::JSONInt(int value) : value(value) {}

std::string JSONInt::toString() const
{
	return  std::to_string(value);
}




JSONString::JSONString(std::string &value) : value(value) {}

std::string JSONString::toString() const
{
	return "\"" + value + "\"";
}

