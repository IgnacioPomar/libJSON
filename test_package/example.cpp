#include <iostream>

#include "libJSON.h"

int main ()
{
	JSONObject jobj;
	jobj.put ("greeting", "hello conan");

	if (JSON_ERR_CODE::SUCCESS != JSONParser::parse (jobj, jobj.toString ().c_str ()))
	{
		return 1;
	}

	std::cout << jobj.toString () << std::endl;
	return 0;
}
