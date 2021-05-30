

#include "JSONBasicTypes.h"
#include "JSONObject.h"
#include "JSONArray.h"

#include "JSONParser.h"


JSON_ERR_CODE JSONParser::parse (JSONArray & base, const char * ini, const char *& lastProcessed)
{
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
