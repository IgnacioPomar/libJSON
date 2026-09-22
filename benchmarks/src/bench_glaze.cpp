#include "bench_common.h"

#include <utility>

#include <glaze/glaze.hpp>

using glz::json_t;


static std::string glazeWrite (const CandleDataset& dataset)
{
	json_t root;
	root["symbol"] = dataset.symbol;

	auto& hoursArr = (root["hours"] = json_t::array_t{}).get_array ();
	hoursArr.reserve (dataset.hours.size ());

	for (const HourGroup& group : dataset.hours)
	{
		json_t hourObj;
		hourObj["hour"] = group.hour;

		auto& candlesArr = (hourObj["candles"] = json_t::array_t{}).get_array ();
		candlesArr.reserve (group.candles.size ());

		for (const Candle& c : group.candles)
		{
			json_t candleObj;
			candleObj["minute"] = c.minute;
			candleObj["open"] = c.open;
			candleObj["high"] = c.high;
			candleObj["low"] = c.low;
			candleObj["close"] = c.close;
			candlesArr.push_back (std::move (candleObj));
		}

		hoursArr.push_back (std::move (hourObj));
	}

	auto result = glz::write_json (root);
	return result ? *result : std::string ();
}

static double glazeRead (const std::string& text)
{
	json_t root;
	[[maybe_unused]] auto ec = glz::read_json (root, text);

	double sum = 0.0;
	for (const json_t& hourVal : root["hours"].get_array ())
	{
		for (const json_t& candleVal : hourVal["candles"].get_array ())
		{
			sum += candleVal["open"].get_number ();
			sum += candleVal["high"].get_number ();
			sum += candleVal["low"].get_number ();
			sum += candleVal["close"].get_number ();
		}
	}

	return sum;
}

JsonLibraryAdapter makeGlazeAdapter ()
{
	return {"glaze (DOM: json_t)", &glazeWrite, &glazeRead};
}
