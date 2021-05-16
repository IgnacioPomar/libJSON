/*********************************************************************************************
*	Name		: JSONBasicTypes.h
*	Description	: Keeps a named array
********************************************************************************************/


#pragma once
#ifndef _JSON_BASIC_TYPES_
#define _JSON_BASIC_TYPES_

#include "libJSON_cfg.h"
#include "JSONBase.h"


class LIBJSON_LOCAL JSONInt : public JSONBase
{
private:
	int value;
public:
	JSONInt(int value);
	std::string toString() const;
};

class LIBJSON_LOCAL JSONString : public JSONBase
{
private:
	std::string value;
public:
	JSONString(std::string & value);
	std::string toString() const;


};



#endif //_JSON_BASIC_TYPES_
