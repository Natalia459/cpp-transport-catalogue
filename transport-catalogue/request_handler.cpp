#include "request_handler.h"

#include <map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>


using namespace std;

using namespace transport_catalogue;
using namespace output;
using namespace types;


//================================================================================================= OUTPUT


void output::GetRequest(std::ostream& out, const std::vector<json::Node>& requests, picture::Renderer render, TransportCatalogue& catalogue) {
	json::Array res;

	for (const auto& info : requests) {
		if (info.AsMap().empty()) {
			continue;
		}
		int id = info.AsMap().at("id"s).AsInt();

		if (info.AsMap().at("type"s).AsString() == "Bus"s) {
			string name = info.AsMap().at("name"s).AsString();
			auto bus = Bus(id, move(name), catalogue);
			res.push_back(bus);
		}
		else if (info.AsMap().at("type"s).AsString() == "Stop"s) {
			string name = info.AsMap().at("name"s).AsString();
			auto stop = Stop(id, move(name), catalogue);
			res.push_back(stop);
		}
		else if (info.AsMap().at("type"s).AsString() == "Map"s) {
			std::ostringstream ostr;
			RenderVector(ostr, render);
			
			json::Dict mapa = { {"map"s, ostr.str()}, {"request_id"s, id}};
			res.push_back(mapa);
		}
	}
	json::Print(json::Document{ res }, out);
}


json::Dict output::Bus(int id, std::string&& bus, TransportCatalogue& catalogue) {

	auto [exists, info] = catalogue.GetBusInfo(move(bus));
	if (exists) {
		json::Dict bus_info;

		bus_info.emplace("request_id"s, id);
		bus_info.emplace("curvature"s, info.curv);
		bus_info.emplace("route_length"s, info.length);
		bus_info.emplace("stop_count"s, info.stops);
		bus_info.emplace("unique_stop_count"s, info.uniq_stops);

		return bus_info;
	}
	else {
		return json::Dict{ { "request_id"s, id}, {"error_message"s, "not found"s} };
	}
}


json::Dict output::Stop(int id, string&& stop, TransportCatalogue& catalogue) {

	auto [exists, buses] = catalogue.GetStopInfo(move(stop));
	if (exists) {
		json::Array buses_json;
		std::sort(buses.begin(), buses.end());
		for (auto bus : buses) {
			buses_json.push_back(static_cast<string>(bus));
		}

		json::Dict stop_info{ {"buses"s, buses_json}, {"request_id"s, id} };

		return stop_info;
	}
	else {
		return json::Dict{ { "request_id"s, id}, {"error_message"s, "not found"s} };
	}
}