﻿/*********************************************************************************************
*	Name		: JSONBase.h
*	Description	: Keeps the array Values
********************************************************************************************/


#pragma once
#ifndef _JSON_BASE_
#define _JSON_BASE_


#include <string>
#include <memory>

#include "libJSON_cfg.h"


class  JSONObject;
class  JSONArray;

class LIBJSON_API JSONBase
{
public:
	virtual std::string toString () const = 0;

};

typedef std::unique_ptr<JSONBase> PtrJSONBase;




enum class JSON_ERR_CODE { SUCCESS, NO_MATCHING_OBJECT, BAD_FORMAT_UNEXPECTED_VALUE,WRONG_STRING_FORMAT,WRONG_NUMBER_FORMAT, UNEXPECTED_END_OF_STRING, NOT_IMPLEMENTED };





#endif //_JSON_BASE_
