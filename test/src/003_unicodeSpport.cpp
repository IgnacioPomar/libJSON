/*********************************************************************************************
*	Name        : 002_parseJSON.cpp
*  Description : Test creating JSON from vars
********************************************************************************************/

#include <TinyCppUnit/TinyCppUnit.h>

#include "JSONParseUchar.h"


UNIT_TEST_CASE (TestEmbeddedUnicode)
{
	//std::string jsonString = "Le\\u00F1a:\\u00E1\\u00E9\\u00ED\\u00F3\\u00FA";
	//std::string utf8String = "\x4C\x65\xC3\xB1\x61\x3a\xC3\xA1\xC3\xA9\xC3\xAD\xC3\xB3\xC3\xBA";



	//Check Parse one unicode char
	std::string ucharStr = "00F1";
	std::string utf8Str = "\xC3\xB1";

	std::string parsed = JSONParseUchar::getUTF8Char (ucharStr);


	UNIT_CHECK (parsed.compare (utf8Str) == 0);


}
