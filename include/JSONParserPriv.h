/*********************************************************************************************
*	Name		: JSONParserPriv.h
*	Description	: Real parser: its propuse is to hide it from the library interface
********************************************************************************************/


#pragma once
#ifndef _JSON_PARSER_PRIVATE_
#define _JSON_PARSER_PRIVATE_

#include <string>

#include "JSONBasicTypes.h"
#include "libJSON_cfg.h"
#include "JSONBase.h"




class LIBJSON_LOCAL JSONParserPriv
{
private:
	static constexpr const char * strNull = "null";
	static constexpr const char * strTrue = "true";
	static constexpr const char * strFalse = "false";
public:
	static bool skipWhitespace (const char *& cursor);
	static bool checkNull (const char *& cursor, JSON_ERR_CODE & errCode);
	static bool checkBool (const char *& cursor, JSON_ERR_CODE & errCode);
	static std::string getStr (const char*& cursor, JSON_ERR_CODE& errCode);
	static std::string getNumber (const char*& cursor, JSON_ERR_CODE& errCode, bool &isInt);

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
