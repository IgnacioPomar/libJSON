/*********************************************************************************************
*	Name		: JSONBase.cpp
*	Description	: Basic conversions
********************************************************************************************/

#include <memory>
#include "JSONArray.h"
#include "JSONObject.h"
#include "JSONBasicTypes.h"
//#include "JSONBase.h" //alreadey included in JSONBasicTypes

const char * JSONBase::getAsString () const
{
	if (this->getType () == JSON_TYPE::JSTRING)
	{
		const JSONString * str = static_cast <const JSONString *> (this);
		return str->value.c_str ();
	}
	else
	{
		return nullptr;
	}
}


int JSONBase::getAsInt () const
{
	if (this->getType () == JSON_TYPE::JINT)
	{
		const JSONInt * obj = static_cast <const JSONInt *> (this);
		return obj->value;
	}
	else
	{
		return 0;
	}
}

bool JSONBase::getAsBool () const
{
	if (this->getType () == JSON_TYPE::JBOOL)
	{
		const JSONBool * obj = static_cast <const JSONBool *> (this);
		return obj->value;
	}
	else
	{
		return false;
	}
}

double JSONBase::getAsDouble () const
{
	if (this->getType () == JSON_TYPE::JDOUBLE)
	{
		const JSONDouble * obj = static_cast <const JSONDouble *> (this);
		return obj->value;
	}
	else if (this->getType () == JSON_TYPE::JINT)
	{
		const JSONInt * obj = static_cast <const JSONInt *> (this);
		return obj->value;
	}
	else
	{
		return 0.0;
	}
}



JSONArray JSONBase::getAsArray () const
{
	if (this->getType () == JSON_TYPE::JARR)
	{
		const JSONArray * arr = static_cast <const JSONArray *> (this);
		return JSONArray (*arr);
	}
	else
	{
		return JSONArray ();
	}
}


JSONObject JSONBase::getAsObject () const
{
	if (this->getType () == JSON_TYPE::JOBJ)
	{
		const JSONObject * obj = static_cast <const JSONObject *> (this);
		return JSONObject (*obj);
	}
	else
	{
		return JSONObject ();
	}
}
