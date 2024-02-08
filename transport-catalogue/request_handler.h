#pragma once
#include "transport_catalogue.h"
#include "map_renderer.h"
#include "json.h"
#include "json_builder.h"
#include "transport_router.h"

#include <string_view>
#include <sstream>

namespace transport_catalogue {

	class RequestHandler {
	public:
		RequestHandler(TransportCatalogue& catalogue, picture::Renderer render, const router::TransportRouter& router)
			:catalogue_(catalogue), render_(std::move(render)), router_(router)
		{
		}

		void GetRequest(std::ostream& out, const json::Node& requests);

	private:
		TransportCatalogue& catalogue_;
		picture::Renderer render_;
		const router::TransportRouter& router_;
		json::Builder req_answer_;
		int id_;


		void Bus(std::string&& bus);

		void Stop(std::string&& stop);

		void Render();

		void Route(std::string_view from_stop, std::string_view to_stop);
	};
}