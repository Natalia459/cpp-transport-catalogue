#pragma once
#include "transport_catalogue.h"

#include <string_view>
#include <sstream>
namespace transport_catalogue {

	namespace output {

		void Request(std::istream& in, std::ostream& out, TransportCatalogue& catalogue);

		void StopsForBus(std::ostream& out, std::string&& bus, TransportCatalogue& catalogue);

		void BusesToStop(std::ostream& out, std::string&& stop, TransportCatalogue& catalogue);
	}
}