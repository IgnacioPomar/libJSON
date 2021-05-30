/*********************************************************************************************
*	Name		: JSONParser.h
*	Description	: Utility class to parse strings
********************************************************************************************/


#pragma once
#ifndef _JSON_PARSER_
#define _JSON_PARSER_

#include "libJSON_cfg.h"
#include "JSONBase.h"


enum JSON_ERR_CODE { SUCCESS, NO_MATCHING_OBJECT, NOT_IMPLEMENTED };

class LIBJSON_API JSONParser
{
public:
	static JSON_ERR_CODE parse (JSONArray& base, const char * jsonTxt);
	static JSON_ERR_CODE parse (JSONObject& base, const char * jsonTxt);
};



#endif //_JSON_PARSER_
