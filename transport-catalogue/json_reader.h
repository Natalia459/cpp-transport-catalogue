#pragma once
#include "transport_catalogue.h"
#include "json.h"

#include <string>

namespace transport_catalogue {

	void Data(std::istream& in, std::ostream& out, TransportCatalogue& catalogue);

	namespace input {

		struct DistanceForStops {
			std::string first;
			std::string second;
			double dist;
		};

		std::vector<types::Buses*> AddData(const json::Array& data, TransportCatalogue& catalogue);

		void Stop(const json::Dict& stop, TransportCatalogue& catalogue);

		void Bus(const json::Dict& bus, TransportCatalogue& catalogue);

		void Distances(std::string&& stop, std::string&& another_stop, double dist, TransportCatalogue& catalogue);
	}
}