#pragma once
#include "transport_catalogue.h"

#include <sstream>

namespace transport_catalogue {

	namespace input {

		struct DistanceForStops {
			std::string first;
			std::string second;
			double dist;
		};

		void Data(std::istream& in, TransportCatalogue& catalogue);

		void Stop(TransportCatalogue& catalogue, std::string&& line, std::vector<DistanceForStops>& stop_distances);

		void Bus(TransportCatalogue& catalogue, std::string&& line);

		void Distances(TransportCatalogue& catalogue, std::string&& stop, std::string&& another_stop, double dist);
	}
}