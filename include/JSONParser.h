/*********************************************************************************************
*	Name		: JSONParser.h
*	Description	: Utility class to parse strings
********************************************************************************************/


#pragma once
#ifndef _JSON_PARSER_
#define _JSON_PARSER_

#include "libJSON_cfg.h"
#include "JSONBase.h"




class LIBJSON_API JSONParser
{
public:
	static JSON_ERR_CODE parse (JSONBase& base, const char * jsonTxt);

	static JSON_ERR_CODE parseFromFile (JSONBase& base, const char * jsonFileName);
};



#endif //_JSON_PARSER_
