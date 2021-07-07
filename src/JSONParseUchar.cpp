/*********************************************************************************************
*	Name		: JSONParseUchar.h
*	Description	: Just to allow UNIT test over the conversion betwenn ncoded chard and UTF8
********************************************************************************************/


#include "JSONParseUchar.h"


#include <locale>
#include <codecvt>


//Microsoft builds std::codecvt against __int32 instead of char32_t
#ifdef _MSC_VER
#define baseType	__int32
#else
#define baseType	char32_t
#endif

std::string JSONParseUchar::getUTF8Char (std::string & uChar)
{
	int i = std::stoi (uChar, 0, 16);


	std::wstring_convert<std::codecvt_utf8<baseType>, baseType> converter;
	std::string u8str = converter.to_bytes (i);

	return u8str;
}
