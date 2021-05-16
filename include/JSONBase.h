/*********************************************************************************************
*	Name		: JSONBase.h
*	Description	: Keeps the array Values
********************************************************************************************/


#pragma once
#ifndef _JSON_BASE_
#define _JSON_BASE_

#include <unordered_map>
#include <forward_list>
#include <string>
#include <memory>

#include "libJSON_cfg.h"


class  JSONObject;
class  JSONArray;

class LIBJSON_API JSONBase
{
public:
	virtual std::string toString() const = 0;

};

typedef std::unique_ptr<JSONBase> PtrJSONBase;
typedef std::forward_list<PtrJSONBase> ArrContainer;
typedef std::unordered_map<std::string, PtrJSONBase> ObjContainer;






#endif //_JSON_BASE_
