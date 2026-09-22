#include "bench_common.h"

#include <simdjson.h>

using namespace simdjson;


// simdjson's DOM parser has no counterpart JSON-building/serialization API: it is a
// read-only parser by design. There is therefore no "write" measurement for it; see
// referenceSerialize() for how the input text fed to simdjsonRead() is produced.
static double simdjsonRead (const std::string& text)
{
	dom::parser parser;
	dom::element doc = parser.parse (text);

	double sum = 0.0;
	for (dom::element hourElem : dom::array (doc["hours"]))
	{
		for (dom::element candleElem : dom::array (hourElem["candles"]))
		{
			sum += double (candleElem["open"]);
			sum += double (candleElem["high"]);
			sum += double (candleElem["low"]);
			sum += double (candleElem["close"]);
		}
	}

	return sum;
}

JsonLibraryAdapter makeSimdjsonAdapter ()
{
	return {"simdjson (DOM, solo lectura)", nullptr, &simdjsonRead};
}
