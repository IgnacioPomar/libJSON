/*********************************************************************************************
*	Name		: candle_data.h
*	Description	: Library-agnostic OHLC candle dataset used as input for every benchmark
********************************************************************************************/

#pragma once

#include <cstdint>
#include <string>
#include <vector>


struct Candle
{
	int minute;
	double open;
	double high;
	double low;
	double close;
};

struct HourGroup
{
	int hour;
	std::vector<Candle> candles;
};

struct CandleDataset
{
	std::string symbol;
	std::vector<HourGroup> hours;
};

// Random-walk OHLC minute candles, grouped by hour. numHours * 60 candles in total.
CandleDataset generateCandleDataset (const std::string& symbol, int numHours, uint32_t seed);

// A single hour group with numMinutes 1-minute candles, used as the "small JSON" case.
CandleDataset generateSmallDataset (const std::string& symbol, uint32_t seed, int numMinutes = 10);

// Library-agnostic serialization (plain string building, no JSON library involved),
// used to produce input text for libraries that can only parse (e.g. simdjson) without
// coupling that measurement to another library's writer.
std::string referenceSerialize (const CandleDataset& dataset);
