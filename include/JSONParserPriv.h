/*********************************************************************************************
*	Name		: JSONParserPriv.h
*	Description	: Real parser: its propuse is to hide it from the library interface
********************************************************************************************/


#pragma once
#ifndef _JSON_PARSER_PRIVATE_
#define _JSON_PARSER_PRIVATE_

#include "libJSON_cfg.h"
#include "JSONBase.h"




class LIBJSON_LOCAL JSONParserPriv
{
public:
	static bool skipWhitespace (const char *& cursor);

	static JSON_ERR_CODE parse (JSONArray& base, const char *& cursor);

	static JSON_ERR_CODE addNewArray (JSONArray& base, const char *& cursor);
	static JSON_ERR_CODE addNewObject (JSONArray& base, const char *& cursor);
	static JSON_ERR_CODE addNewString (JSONArray& base, const char *& cursor);
	static JSON_ERR_CODE addNewNumber (JSONArray& base, const char *& cursor);
	static JSON_ERR_CODE addNewBoolean (JSONArray& base, const char *& cursor);
	static JSON_ERR_CODE addNewNull (JSONArray& base, const char *& cursor);




	static JSON_ERR_CODE parse (JSONObject& base, const char *& cursor);
};



#endif //_JSON_PARSER_PRIVATE_
