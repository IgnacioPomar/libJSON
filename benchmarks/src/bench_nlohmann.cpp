#include "bench_common.h"

#include <nlohmann/json.hpp>

using json = nlohmann::json;


static std::string nlohmannWrite (const CandleDataset& dataset)
{
	json root;
	root["symbol"] = dataset.symbol;
	root["hours"] = json::array ();

	for (const HourGroup& group : dataset.hours)
	{
		json hourObj;
		hourObj["hour"] = group.hour;
		hourObj["candles"] = json::array ();

		for (const Candle& c : group.candles)
		{
			hourObj["candles"].push_back ({
				{"minute", c.minute},
				{"open", c.open},
				{"high", c.high},
				{"low", c.low},
				{"close", c.close}
			});
		}

		root["hours"].push_back (std::move (hourObj));
	}

	return root.dump ();
}

static double nlohmannRead (const std::string& text)
{
	json root = json::parse (text);

	double sum = 0.0;
	for (const auto& hourObj : root["hours"])
	{
		for (const auto& candleObj : hourObj["candles"])
		{
			sum += candleObj["open"].get<double> ();
			sum += candleObj["high"].get<double> ();
			sum += candleObj["low"].get<double> ();
			sum += candleObj["close"].get<double> ();
		}
	}

	return sum;
}

JsonLibraryAdapter makeNlohmannAdapter ()
{
	return {"nlohmann/json (DOM)", &nlohmannWrite, &nlohmannRead};
}
