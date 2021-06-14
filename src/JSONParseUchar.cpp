/*********************************************************************************************
*	Name		: JSONParseUchar.h
*	Description	: Just to allow UNIT test over the conversion betwenn ncoded chard and UTF8
********************************************************************************************/


#include "JSONParseUchar.h"


#include <locale>
#include <codecvt>


std::string JSONParseUchar::getUTF8Char (std::string & uChar)
{
	int i = std::stoi (uChar, 0, 16);


	std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
	std::string u8str = converter.to_bytes (i);

	return u8str;
}
