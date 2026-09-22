#include "candle_data.h"

#include <algorithm>
#include <random>


static CandleDataset generateWalk (const std::string& symbol, int numHours, int candlesPerHour, uint32_t seed)
{
	std::mt19937 rng (seed);
	std::uniform_real_distribution<double> stepDist (-1.0, 1.0);
	std::uniform_real_distribution<double> wickDist (0.0, 0.5);

	CandleDataset dataset;
	dataset.symbol = symbol;
	dataset.hours.reserve (numHours);

	double lastClose = 100.0;
	int minuteCounter = 0;

	for (int hour = 0; hour < numHours; ++hour)
	{
		HourGroup group;
		group.hour = hour;
		group.candles.reserve (candlesPerHour);

		for (int i = 0; i < candlesPerHour; ++i)
		{
			Candle candle;
			candle.minute = minuteCounter++;
			candle.open = lastClose;
			candle.close = candle.open + stepDist (rng);
			candle.high = std::max (candle.open, candle.close) + wickDist (rng);
			candle.low = std::min (candle.open, candle.close) - wickDist (rng);

			lastClose = candle.close;
			group.candles.push_back (candle);
		}

		dataset.hours.push_back (std::move (group));
	}

	return dataset;
}

CandleDataset generateCandleDataset (const std::string& symbol, int numHours, uint32_t seed)
{
	return generateWalk (symbol, numHours, 60, seed);
}

CandleDataset generateSmallDataset (const std::string& symbol, uint32_t seed, int numMinutes)
{
	return generateWalk (symbol, 1, numMinutes, seed);
}

std::string referenceSerialize (const CandleDataset& dataset)
{
	std::string out;
	out.reserve (64 + dataset.hours.size () * 60 * 96);

	out += "{\"symbol\":\"";
	out += dataset.symbol;
	out += "\",\"hours\":[";

	for (size_t h = 0; h < dataset.hours.size (); ++h)
	{
		const HourGroup& group = dataset.hours[h];
		if (h > 0) out += ',';

		out += "{\"hour\":";
		out += std::to_string (group.hour);
		out += ",\"candles\":[";

		for (size_t i = 0; i < group.candles.size (); ++i)
		{
			const Candle& c = group.candles[i];
			if (i > 0) out += ',';

			out += "{\"minute\":";
			out += std::to_string (c.minute);
			out += ",\"open\":";
			out += std::to_string (c.open);
			out += ",\"high\":";
			out += std::to_string (c.high);
			out += ",\"low\":";
			out += std::to_string (c.low);
			out += ",\"close\":";
			out += std::to_string (c.close);
			out += '}';
		}

		out += "]}";
	}

	out += "]}";
	return out;
}
