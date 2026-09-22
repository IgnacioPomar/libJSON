#include "bench_common.h"

#include "libJSON.h"


static std::string libJsonWrite (const CandleDataset& dataset)
{
	JSONObject root;
	root.put ("symbol", dataset.symbol.c_str ());

	JSONArray hoursArr;
	for (const HourGroup& group : dataset.hours)
	{
		JSONObject hourObj;
		hourObj.put ("hour", group.hour);

		JSONArray candlesArr;
		for (const Candle& c : group.candles)
		{
			JSONObject candleObj;
			candleObj.put ("minute", c.minute);
			candleObj.put ("open", c.open);
			candleObj.put ("high", c.high);
			candleObj.put ("low", c.low);
			candleObj.put ("close", c.close);
			candlesArr.put (candleObj);
		}

		hourObj.put ("candles", candlesArr);
		hoursArr.put (hourObj);
	}

	root.put ("hours", hoursArr);
	return root.toString ();
}

static double libJsonRead (const std::string& text)
{
	JSONObject root;
	JSONParser::parse (root, text.c_str ());

	double sum = 0.0;
	JSONArray hoursArr = root.get ("hours")->getAsArray ();
	for (PtrJSONBase& hourElem : hoursArr)
	{
		JSONObject hourObj = hourElem->getAsObject ();
		JSONArray candlesArr = hourObj.get ("candles")->getAsArray ();
		for (PtrJSONBase& candleElem : candlesArr)
		{
			JSONObject candleObj = candleElem->getAsObject ();
			sum += candleObj.get ("open")->getAsDouble ();
			sum += candleObj.get ("high")->getAsDouble ();
			sum += candleObj.get ("low")->getAsDouble ();
			sum += candleObj.get ("close")->getAsDouble ();
		}
	}

	return sum;
}

JsonLibraryAdapter makeLibJSONAdapter ()
{
	return {"libJSON (shared_ptr DOM)", &libJsonWrite, &libJsonRead};
}
