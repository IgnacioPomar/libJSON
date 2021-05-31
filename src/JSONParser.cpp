

#include "JSONBasicTypes.h"
#include "JSONObject.h"
#include "JSONArray.h"

#include "JSONParser.h"


JSON_ERR_CODE JSONParser::parse (JSONArray & base, const char *& lastProcessed)
{
	if (lastProcessed[0] != '[')
	{
		return NO_MATCHING_OBJECT;
	}

	while ((++lastProcessed)[0] != 0)
	{
		switch (lastProcessed[0])
		{
		case ' ':
		case '\t':
		case '\r':
		case '\n':
			continue;
			break;
		case '[':
			//JSONArray jarr;
			//parse (jarr, lastProcessed, lastProcessed);
			break;
		}

	}




	return JSON_ERR_CODE ();
}

JSON_ERR_CODE JSONParser::parse (JSONArray & base, const char * jsonTxt)
{
	return NOT_IMPLEMENTED;
}


JSON_ERR_CODE JSONParser::parse (JSONObject & base, const char * jsonTxt)
{
	return NOT_IMPLEMENTED;
}
