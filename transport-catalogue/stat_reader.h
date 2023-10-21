#pragma once
#include "transport_catalogue.h"

#include <string_view>
#include <sstream>
namespace transport_catalogue {

	namespace output {

		void Request(std::istream&, std::ostream&, TransportCatalogue&);

		void StopsForBus(std::ostream&, std::string&&, TransportCatalogue&);

		void BusesToStop(std::ostream&, std::string&&, TransportCatalogue&);
	}
}