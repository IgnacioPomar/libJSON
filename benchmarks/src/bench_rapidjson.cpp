#include "bench_common.h"

#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>

using namespace rapidjson;


static std::string rapidjsonWrite (const CandleDataset& dataset)
{
	Document doc;
	doc.SetObject ();
	auto& alloc = doc.GetAllocator ();

	Value symbolVal;
	symbolVal.SetString (dataset.symbol.c_str (), static_cast<SizeType> (dataset.symbol.size ()), alloc);
	doc.AddMember ("symbol", symbolVal, alloc);

	Value hoursArr (kArrayType);
	for (const HourGroup& group : dataset.hours)
	{
		Value hourObj (kObjectType);
		hourObj.AddMember ("hour", group.hour, alloc);

		Value candlesArr (kArrayType);
		for (const Candle& c : group.candles)
		{
			Value candleObj (kObjectType);
			candleObj.AddMember ("minute", c.minute, alloc);
			candleObj.AddMember ("open", c.open, alloc);
			candleObj.AddMember ("high", c.high, alloc);
			candleObj.AddMember ("low", c.low, alloc);
			candleObj.AddMember ("close", c.close, alloc);
			candlesArr.PushBack (candleObj, alloc);
		}

		hourObj.AddMember ("candles", candlesArr, alloc);
		hoursArr.PushBack (hourObj, alloc);
	}

	doc.AddMember ("hours", hoursArr, alloc);

	StringBuffer buffer;
	Writer<StringBuffer> writer (buffer);
	doc.Accept (writer);
	return std::string (buffer.GetString (), buffer.GetSize ());
}

static double rapidjsonRead (const std::string& text)
{
	Document doc;
	doc.Parse (text.c_str (), text.size ());

	double sum = 0.0;
	for (const Value& hourObj : doc["hours"].GetArray ())
	{
		for (const Value& candleObj : hourObj["candles"].GetArray ())
		{
			sum += candleObj["open"].GetDouble ();
			sum += candleObj["high"].GetDouble ();
			sum += candleObj["low"].GetDouble ();
			sum += candleObj["close"].GetDouble ();
		}
	}

	return sum;
}

JsonLibraryAdapter makeRapidJsonAdapter ()
{
	return {"RapidJSON (DOM)", &rapidjsonWrite, &rapidjsonRead};
}
