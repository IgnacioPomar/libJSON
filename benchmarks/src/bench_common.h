/*********************************************************************************************
*	Name		: bench_common.h
*	Description	: Common interface every library adapter implements
********************************************************************************************/

#pragma once

#include <string>

#include "candle_data.h"


struct JsonLibraryAdapter
{
	const char* name;

	// Builds the DOM from the dataset and serializes it to text.
	std::string (*write) (const CandleDataset& dataset);

	// Parses the text back into a DOM and walks it fully, returning the sum of all
	// open+high+low+close values (used to sanity-check correctness and to stop the
	// compiler from optimizing the parse away).
	double (*read) (const std::string& text);
};

JsonLibraryAdapter makeLibJSONAdapter ();
JsonLibraryAdapter makeNlohmannAdapter ();
JsonLibraryAdapter makeRapidJsonAdapter ();
JsonLibraryAdapter makeSimdjsonAdapter ();
JsonLibraryAdapter makeGlazeAdapter ();
