#pragma once
#include "transport_catalogue.h"
#include  "map_renderer.h"
#include "json.h"

#include <string_view>
#include <sstream>

namespace transport_catalogue {

	namespace output {

		void GetRequest(std::ostream& out, const std::vector<json::Node>& requests, picture::Renderer, TransportCatalogue& catalogue);

		json::Dict Bus(int id, std::string&& bus, TransportCatalogue& catalogue);

		json::Dict Stop(int id,  std::string&& stop, TransportCatalogue& catalogue);
	}
}