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

		void Data(std::istream&, TransportCatalogue&);

		void Stop(TransportCatalogue&, std::string&&, std::vector<DistanceForStops>&);

		void Bus(TransportCatalogue&, std::string&&);

		void Distances(TransportCatalogue&, std::string&&, std::string&&, double);
	}
}