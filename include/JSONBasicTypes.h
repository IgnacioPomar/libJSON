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
public:
	int value;

	JSONInt (int value);
	std::string toString () const;
	JSON_TYPE getType () const;
};

class LIBJSON_LOCAL JSONBool : public JSONBase
{
private:
public:
	bool value;


	JSONBool (bool value);
	std::string toString () const;
	JSON_TYPE getType () const;
};

class LIBJSON_LOCAL JSONNull : public JSONBase
{
private:
public:
	std::string toString () const;
	JSON_TYPE getType () const;
};


class LIBJSON_LOCAL JSONDouble : public JSONBase
{
private:
public:
	double value;

	JSONDouble (double value);
	std::string toString () const;
	JSON_TYPE getType () const;
};

class LIBJSON_LOCAL JSONString : public JSONBase
{
public:
	std::string value;

	JSONString (std::string & value);
	std::string toString () const;
	JSON_TYPE getType () const;
};



#endif //_JSON_BASIC_TYPES_
