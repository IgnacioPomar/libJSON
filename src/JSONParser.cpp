

#include "JSONBasicTypes.h"
#include "JSONObject.h"
#include "JSONArray.h"

#include "JSONParser.h"
#include "JSONParserPriv.h"


JSON_ERR_CODE JSONParser::parse (JSONArray & base, const char * jsonTxt)
{
	const char * cursor = jsonTxt;
	return JSONParserPriv::parse (base, cursor);
}


JSON_ERR_CODE JSONParser::parse (JSONObject & base, const char * jsonTxt)
{
	const char * cursor = jsonTxt;
	return JSONParserPriv::parse (base, cursor);
}



/**
* Advances the position till we find a non whitespace char
*/
bool JSONParserPriv::skipWhitespace (const char *& cursor)
{
	bool retVal = false;
	while (cursor[0] == ' ' || cursor[0] == '\t' || cursor[0] == '\r' || cursor[0] == '\n')
	{
		cursor++;
		retVal = true;
	}

	return retVal;
}


/**
* check if we hava a null string and advance the cursor till the end
*/
bool JSONParserPriv::checkNull (const char *& cursor, JSON_ERR_CODE & errCode)
{
	const char * tmp = strNull;

	errCode = JSON_ERR_CODE::SUCCESS;
	while (tmp[0] != 0)
	{
		if (tmp[0] != cursor[0])
		{
			errCode = JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE;
			return false;
		}
		else
		{
			cursor++;
			tmp++;
		}
	}

	return true;
}

bool JSONParserPriv::checkBool (const char *& cursor, JSON_ERR_CODE & errCode)
{
	bool retVal = (cursor[0] == 't');
	const char * tmp = (retVal) ? strTrue : strFalse;

	errCode = JSON_ERR_CODE::SUCCESS;
	while (tmp[0] != 0)
	{
		if (tmp[0] != cursor[0])
		{
			errCode = JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE;
			return false;
		}
		else
		{
			cursor++;
			tmp++;
		}
	}

	return retVal;
}


std::string JSONParserPriv::getStr (const char*& cursor, JSON_ERR_CODE& errCode)
{
	std::string retVal = "";

	//Un string debe empezar por comillas y terminar por comillas:
	if (cursor[0] != '"')
	{
		errCode=  JSON_ERR_CODE::WRONG_STRING_FORMAT;
		return retVal;
	}

	//Avanzamos para ir al siguiente caracter
	cursor++;
	errCode = JSON_ERR_CODE::SUCCESS;


	bool insideScape = false;
	int remainingUChars = 0;
	std::string uchar;

	//Comenzamos el parseo del string
	while (cursor[0] != 0 && cursor[0] != '"' && errCode == JSON_ERR_CODE::SUCCESS)
	{
		if (insideScape)
		{
			insideScape = false;
			switch (cursor[0])
			{
			case '"': retVal.push_back ('"'); break;
			case '\\': retVal.push_back ('\\'); break;
			case '/': retVal.push_back ('/'); break;
			case 'b': retVal.push_back ('b'); break;
			case 'f': retVal.push_back ('f'); break;
			case 'n': retVal.push_back ('n'); break;
			case 'r': retVal.push_back ('r'); break;
			case 't': retVal.push_back ('t'); break;
			case 'u': 
				remainingUChars = 4;
				uchar.clear ();
				break;
			default:
				errCode = JSON_ERR_CODE::WRONG_STRING_FORMAT;
				break;
			}
		}
		else if (remainingUChars > 0)
		{
			if (std::isxdigit (cursor[0]))
			{
				uchar.push_back (cursor[0]);
				remainingUChars--;

				if (remainingUChars == 0)
				{
					retVal.append (getUTF8Char (uchar));
				}
			}
			else
			{
				errCode = JSON_ERR_CODE::WRONG_STRING_FORMAT;
			}
		}
		else
		{
			if (cursor[0] == '\\')
			{
				insideScape = true;
			}
			else
			{
				retVal.push_back (cursor[0]);
			}
		}
		cursor++;
	}

	if (insideScape || remainingUChars > 0 || cursor[0] != '"')
	{
		errCode = JSON_ERR_CODE::WRONG_STRING_FORMAT;
	}
	else
	{
		cursor++;
	}

	return retVal;
}


std::string JSONParserPriv::getUTF8Char (std::string uChar)
{
	//TODO: convertir de caracter unicode puro a utf8
	return std::string ("");
}


JSON_ERR_CODE JSONParserPriv::addNewBoolean (JSONArray & base, const char *& cursor)
{
	JSON_ERR_CODE retVal;
	base.put (checkBool (cursor, retVal));
	return retVal;
}


JSON_ERR_CODE JSONParserPriv::addNewNull (JSONArray & base, const char *& cursor)
{
	JSON_ERR_CODE retVal;
	if (checkNull (cursor, retVal))
	{
		base.put (0); //In C++ it will be 0 anyway...
	}
	return retVal;
}


JSON_ERR_CODE JSONParserPriv::addNewArray (JSONArray & base, const char *& cursor)
{
	JSONArray jarr;
	JSON_ERR_CODE retVal = parse (jarr, cursor);
	if (retVal == JSON_ERR_CODE::SUCCESS)
	{
		base.put (jarr);
	}

	return retVal;
}

JSON_ERR_CODE JSONParserPriv::addNewObject (JSONArray & base, const char *& cursor)
{
	JSONObject jObj;
	JSON_ERR_CODE retVal = parse (jObj, cursor);

	if (retVal == JSON_ERR_CODE::SUCCESS)
	{
		base.put (jObj);
	}

	return retVal;
}

JSON_ERR_CODE JSONParserPriv::parse (JSONArray & base, const char *& cursor)
{
	bool waitingForValue = true;
	if (cursor[0] != '[')
	{
		return JSON_ERR_CODE::NO_MATCHING_OBJECT;
	}


	JSON_ERR_CODE retVal = JSON_ERR_CODE::SUCCESS;
	while ((++cursor)[0] != 0 && cursor[0] != ']')
	{
		if (skipWhitespace (cursor))
		{
			//Nothing to do
		}
		else if (waitingForValue)
		{
			switch (cursor[0])
			{
			case '[':
				retVal = addNewArray (base, cursor);
				break;
			case '{':
				retVal = addNewObject (base, cursor);
				break;
			case '"':
				retVal = addNewString (base, cursor);
				break;
			case 't':
			case 'f':
				retVal = addNewBoolean (base, cursor);
				break;
			case 'n':
				retVal = addNewNull (base, cursor);
				break;
			default:
				retVal = addNewNumber (base, cursor);
				break;
			}

			if (retVal != JSON_ERR_CODE::SUCCESS)
			{
				return retVal;
			}

			waitingForValue = false;
		}
		else
		{
			if (cursor[0] == ',')
			{
				waitingForValue = true;
			}
			else
			{
				return JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE;
			}
		}

	}

	return JSON_ERR_CODE::SUCCESS;
}
