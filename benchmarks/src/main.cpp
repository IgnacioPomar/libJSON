#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <numeric>
#include <string>
#include <vector>

#include "bench_common.h"
#include "candle_data.h"


namespace
{
	using Clock = std::chrono::steady_clock;

	double microsecondsBetween (Clock::time_point a, Clock::time_point b)
	{
		return std::chrono::duration<double, std::micro> (b - a).count ();
	}

	double expectedChecksum (const CandleDataset& dataset)
	{
		double sum = 0.0;
		for (const HourGroup& group : dataset.hours)
			for (const Candle& c : group.candles)
				sum += c.open + c.high + c.low + c.close;
		return sum;
	}

	struct Stats
	{
		bool available = false;
		double minUs = 0, maxUs = 0, meanUs = 0;
	};

	Stats computeStats (const std::vector<double>& samples)
	{
		Stats s;
		if (samples.empty ()) return s;
		s.available = true;
		s.minUs = *std::min_element (samples.begin (), samples.end ());
		s.maxUs = *std::max_element (samples.begin (), samples.end ());
		s.meanUs = std::accumulate (samples.begin (), samples.end (), 0.0) / samples.size ();
		return s;
	}

	void printStatsRow (const char* label, const Stats& s)
	{
		if (!s.available)
		{
			std::printf ("  %-14s %10s %10s %10s\n", label, "N/A", "N/A", "N/A");
			return;
		}
		std::printf ("  %-14s %10.1f %10.1f %10.1f\n", label, s.minUs, s.meanUs, s.maxUs);
	}

	struct Dataset
	{
		const char* label;
		CandleDataset data;
	};

	void runBenchmarkFor (const Dataset& dataset, int rounds)
	{
		const std::string canonicalText = referenceSerialize (dataset.data);
		const double expected = expectedChecksum (dataset.data);

		std::printf ("\n=== Dataset: %s (%zu horas, %zu velas, %zu bytes) ===\n",
			dataset.label,
			dataset.data.hours.size (),
			[&] { size_t n = 0; for (auto& h : dataset.data.hours) n += h.candles.size (); return n; }(),
			canonicalText.size ());
		std::printf ("  %-14s %10s %10s %10s  (microsegundos por vuelta, %d vueltas)\n",
			"", "min", "media", "max", rounds);

		JsonLibraryAdapter adapters[] = {
			makeLibJSONAdapter (),
			makeNlohmannAdapter (),
			makeRapidJsonAdapter (),
			makeSimdjsonAdapter (),
			makeGlazeAdapter (),
		};

		for (JsonLibraryAdapter& adapter : adapters)
		{
			std::vector<double> writeUs;
			std::vector<double> readUs;
			bool checksumOk = true;

			for (int round = 0; round < rounds; ++round)
			{
				std::string text;

				if (adapter.write != nullptr)
				{
					Clock::time_point t0 = Clock::now ();
					text = adapter.write (dataset.data);
					Clock::time_point t1 = Clock::now ();
					writeUs.push_back (microsecondsBetween (t0, t1));
				}
				else
				{
					text = canonicalText;
				}

				Clock::time_point t2 = Clock::now ();
				double sum = adapter.read (text);
				Clock::time_point t3 = Clock::now ();
				readUs.push_back (microsecondsBetween (t2, t3));

				if (std::fabs (sum - expected) > 1e-6 * std::max (1.0, std::fabs (expected)))
					checksumOk = false;
			}

			std::printf (" %s%s\n", adapter.name, checksumOk ? "" : "   [!] checksum mismatch");
			printStatsRow ("escritura", computeStats (writeUs));
			printStatsRow ("lectura", computeStats (readUs));
		}
	}
}

int main (int argc, char** argv)
{
	int rounds = 20;
	if (argc > 1) rounds = std::max (1, std::atoi (argv[1]));

	std::vector<Dataset> datasets;
	datasets.push_back ({"pequeño (10 min)", generateSmallDataset ("BENCH", 1234, 10)});
	datasets.push_back ({"medio (1 día minutal)", generateCandleDataset ("BENCH", 24, 5678)});
	datasets.push_back ({"grande (1 semana minutal)", generateCandleDataset ("BENCH", 24 * 7, 9012)});

	std::printf ("libJSON JSON benchmark - %d vueltas por combinacion libreria/dataset\n", rounds);

	for (const Dataset& dataset : datasets)
		runBenchmarkFor (dataset, rounds);

	return 0;
}
