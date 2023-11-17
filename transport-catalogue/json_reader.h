#pragma once
#include "transport_catalogue.h"
#include "json.h"

#include <string>

namespace transport_catalogue {

	class JSONReader {
	public:
		struct DistanceForStops {
			std::string first;
			std::string second;
			double dist;
		};


		JSONReader(TransportCatalogue& catalogue)
			:catalogue_(catalogue)
		{
		}

		void ProcessData(std::istream& in, std::ostream& out);

		std::vector<types::Buses*> AddData(const json::Array& data);

	private:
		TransportCatalogue& catalogue_;


		void Stop(const json::Dict& stop);

		void Bus(const json::Dict& bus);

		void Distances(std::string&& stop, std::string&& another_stop, double dist);
	};
}