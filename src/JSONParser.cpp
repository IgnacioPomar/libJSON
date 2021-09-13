
#include <cctype>

#include "JSONParseUchar.h"
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
		errCode = JSON_ERR_CODE::WRONG_STRING_FORMAT;
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
					retVal.append (JSONParseUchar::getUTF8Char (uchar));
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

std::string JSONParserPriv::getNumber (const char*& cursor, JSON_ERR_CODE& errCode, bool& isInt)
{
	//Esta funcion se encarga exclusivamente de "agrupar" el número y saber que tipo de número es
	std::string retVal = "";

	//Debe contener al menos un caracter en la parte entera
	errCode = JSON_ERR_CODE::WRONG_NUMBER_FORMAT;

	isInt = true;

	//Opcionalmente puede empezar por el signo -
	if (cursor[0] == '-')
	{
		retVal.push_back ('-');
		cursor++;
	}

	//Primero viene la parte entera
	while (std::isdigit (cursor[0]))
	{
		retVal.push_back (cursor[0]);
		cursor++;

		errCode = JSON_ERR_CODE::SUCCESS;
	}


	//Ahora viene la fracción si es el caso
	if (cursor[0] != '.')
	{
		return retVal;
	}
	else
	{
		isInt = false;
		retVal.push_back ('.');
		cursor++;

		//Si existe la parte de fracción, debe existir al menos un dígito
		errCode = JSON_ERR_CODE::WRONG_NUMBER_FORMAT;
	}

	while (std::isdigit (cursor[0]))
	{
		retVal.push_back (cursor[0]);
		cursor++;

		errCode = JSON_ERR_CODE::SUCCESS;
	}

	//Ahora viene la parte exponencial, de ser el caso
	if (cursor[0] != 'e' && cursor[0] != 'E')
	{
		return retVal;
	}
	else
	{
		retVal.push_back ('E');
		cursor++;

		//Si existe la parte de exponente, debe existir al menos un dígito
		errCode = JSON_ERR_CODE::WRONG_NUMBER_FORMAT;
	}

	//Puede ser que exista (o que no), signo para el exponente
	if (cursor[0] == '+' || cursor[0] == '-')
	{
		retVal.push_back (cursor[0]);
		cursor++;
	}

	while (std::isdigit (cursor[0]))
	{
		retVal.push_back (cursor[0]);
		cursor++;

		errCode = JSON_ERR_CODE::SUCCESS;
	}


	return retVal;
}

// ----------------------------------------------------------------------
// ----------------------------- JSONArray -----------------------------
// ----------------------------------------------------------------------

JSON_ERR_CODE JSONParserPriv::addNewString (JSONArray& base, const char*& cursor)
{
	JSON_ERR_CODE retVal;
	std::string str = getStr (cursor, retVal);
	base.put (str.c_str ());
	return retVal;
}


JSON_ERR_CODE JSONParserPriv::addNewNumber (JSONArray& base, const char*& cursor)
{
	JSON_ERR_CODE retVal;

	bool isInt;
	std::string number = getNumber (cursor, retVal, isInt);

	if (retVal == JSON_ERR_CODE::SUCCESS)
	{

		if (isInt)
		{
			base.put (std::stoi (number));
		}
		else
		{
			base.put (std::stod (number));
		}
	}


	return retVal;
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
		base.putNull ();
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

	cursor++;

	JSON_ERR_CODE retVal = JSON_ERR_CODE::SUCCESS;
	while ((cursor)[0] != 0 && cursor[0] != ']')
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
				cursor++;
			}
			else
			{
				return JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE;
			}
		}

	}

	if (cursor[0] == ']')
	{
		cursor++;
		return JSON_ERR_CODE::SUCCESS;
	}
	else
	{
		return JSON_ERR_CODE::UNEXPECTED_END_OF_STRING;
	}
}

// ----------------------------------------------------------------------
// ----------------------------- JSONObject -----------------------------
// ----------------------------------------------------------------------



JSON_ERR_CODE JSONParserPriv::addNewString (JSONObject& object, const char* key, const char*& cursor)
{
	JSON_ERR_CODE retVal;
	std::string str = getStr (cursor, retVal);
	object.put (key, str.c_str ());
	return retVal;
}


JSON_ERR_CODE JSONParserPriv::addNewNumber (JSONObject& object, const char* key, const char*& cursor)
{
	JSON_ERR_CODE retVal;

	bool isInt;
	std::string number = getNumber (cursor, retVal, isInt);

	if (retVal == JSON_ERR_CODE::SUCCESS)
	{

		if (isInt)
		{
			object.put (key, std::stoi (number));
		}
		else
		{
			object.put (key, std::stod (number));
		}
	}


	return retVal;
}



JSON_ERR_CODE JSONParserPriv::addNewBoolean (JSONObject& object, const char* key, const char*& cursor)
{
	JSON_ERR_CODE retVal;
	object.put (key, checkBool (cursor, retVal));
	return retVal;
}


JSON_ERR_CODE JSONParserPriv::addNewNull (JSONObject& object, const char* key, const char*& cursor)
{
	JSON_ERR_CODE retVal;
	if (checkNull (cursor, retVal))
	{
		object.putNull (key);
	}
	return retVal;
}


JSON_ERR_CODE JSONParserPriv::addNewArray (JSONObject& object, const char* key, const char*& cursor)
{
	JSONArray jarr;
	JSON_ERR_CODE retVal = parse (jarr, cursor);
	if (retVal == JSON_ERR_CODE::SUCCESS)
	{
		object.put (key, jarr);
	}

	return retVal;
}

JSON_ERR_CODE JSONParserPriv::addNewObject (JSONObject& object, const char* key, const char*& cursor)
{
	JSONObject jObj;
	JSON_ERR_CODE retVal = parse (jObj, cursor);

	if (retVal == JSON_ERR_CODE::SUCCESS)
	{
		object.put (key, jObj);
	}

	return retVal;
}


JSON_ERR_CODE JSONParserPriv::parse (JSONObject& object, const char*& cursor)
{
	bool waitingForKey = true;
	bool waitingForValue = false;
	bool keyIsFilled = false;

	std::string key;

	if (cursor[0] != '{')
	{
		return JSON_ERR_CODE::NO_MATCHING_OBJECT;
	}

	cursor++;

	JSON_ERR_CODE retVal = JSON_ERR_CODE::SUCCESS;
	while ((cursor)[0] != 0 && cursor[0] != '}')
	{
		if (skipWhitespace (cursor))
		{
			//Nothing to do
		}
		else if (waitingForKey)
		{
			key = getStr (cursor, retVal);
			waitingForKey = false;

			if (retVal != JSON_ERR_CODE::SUCCESS)
			{
				return retVal;
			}
		}
		else if (waitingForValue)
		{
			switch (cursor[0])
			{
			case '[':
				retVal = addNewArray (object, key.c_str (), cursor);
				break;
			case '{':
				retVal = addNewObject (object, key.c_str (), cursor);
				break;
			case '"':
				retVal = addNewString (object, key.c_str (), cursor);
				break;
			case 't':
			case 'f':
				retVal = addNewBoolean (object, key.c_str (), cursor);
				break;
			case 'n':
				retVal = addNewNull (object, key.c_str (), cursor);
				break;
			default:
				retVal = addNewNumber (object, key.c_str (), cursor);
				break;
			}

			if (retVal != JSON_ERR_CODE::SUCCESS)
			{
				return retVal;
			}

			keyIsFilled = true;
			waitingForValue = false;
		}
		else
		{
			//If it is neither a whitespace, nor the key, nor the value, then 
			switch (cursor[0])
			{
			case ',':
				if (keyIsFilled)
				{
					waitingForKey = true;
					keyIsFilled = false;
					cursor++;
				}
				else
				{
					return JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE;
				}
				break;
			case ':':
				if (!waitingForKey)
				{
					waitingForValue = true;
					cursor++;
				}
				else
				{
					return JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE;
				}
				break;
			default:
				return JSON_ERR_CODE::BAD_FORMAT_UNEXPECTED_VALUE;
				break;
			}
		}
	}


	if (cursor[0] == '}')
	{
		cursor++;
		return JSON_ERR_CODE::SUCCESS;
	}
	else
	{
		return JSON_ERR_CODE::UNEXPECTED_END_OF_STRING;
	}
}
