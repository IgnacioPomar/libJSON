/*********************************************************************************************
*	Name		: JSONParseUchar.h
*	Description	: Just to allow UNIT test over the conversion betwenn ncoded chard and UTF8
********************************************************************************************/


#pragma once
#ifndef _JSON_PARSE_UCHAR_
#define _JSON_PARSE_UCHAR_

#include <string>

#include "libJSON_cfg.h"



class LIBJSON_API JSONParseUchar
{
public:
	static std::string getUTF8Char (std::string &uChar);
};



#endif //_JSON_PARSE_UCHAR_
