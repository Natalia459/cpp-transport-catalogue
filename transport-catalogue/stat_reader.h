#pragma once
#include "transport_catalogue.h"

#include <string_view>
#include <sstream>
namespace transport_catalogue {

	namespace output {

		void GetRequest(std::istream& in, std::ostream& out, TransportCatalogue& catalogue);

		void Bus(std::ostream& out, std::string&& bus, TransportCatalogue& catalogue);

		void Stop(std::ostream& out, std::string&& stop, TransportCatalogue& catalogue);
	}
}