#pragma once
#include "transport_catalogue.h"
#include "map_renderer.h"
#include "json.h"
#include "json_builder.h"

#include <string_view>
#include <sstream>

namespace transport_catalogue {

	class RequestHandler {
	public:
		RequestHandler(TransportCatalogue& catalogue)
			:catalogue_(catalogue)
		{
		}

		void GetRequest(std::ostream& out, const json::Node& requests, picture::Renderer);

	private:
		TransportCatalogue& catalogue_;
		json::Builder req_answer_;


		void Bus(int id, std::string&& bus);

		void Stop(int id, std::string&& stop);
	};
}