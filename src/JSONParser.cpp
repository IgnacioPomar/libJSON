

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


JSON_ERR_CODE JSONParserPriv::addNewArray (JSONArray & base, const char *& cursor)
{
	JSONArray jarr;
	JSON_ERR_CODE retVal = parse (jarr, cursor);
	if (retVal == SUCCESS)
	{
		base.put (jarr);
	}

	return retVal;
}

JSON_ERR_CODE JSONParserPriv::addNewObject (JSONArray & base, const char *& cursor)
{
	JSONObject jObj;
	JSON_ERR_CODE retVal = parse (jObj, cursor);

	if (retVal == SUCCESS)
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
		return NO_MATCHING_OBJECT;
	}


	JSON_ERR_CODE retVal = SUCCESS;
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

			if (retVal != SUCCESS)
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
				return BAD_FORMAT_UNEXPECTED_VALUE;
			}
		}

	}

	return SUCCESS;
}
