#include "request_handler.h"

#include <map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>


using namespace std;

using namespace transport_catalogue;
using namespace types;


//================================================================================================= OUTPUT


void RequestHandler::GetRequest(std::ostream& out, const std::vector<json::Node>& requests, picture::Renderer render) {

	for (const auto& info : requests) {
		if (info.AsMap().empty()) {
			continue;
		}
		int id = info.AsMap().at("id"s).AsInt();

		if (info.AsMap().at("type"s).AsString() == "Bus"s) {
			string name = info.AsMap().at("name"s).AsString();
			Bus(id, move(name));
		}
		else if (info.AsMap().at("type"s).AsString() == "Stop"s) {
			string name = info.AsMap().at("name"s).AsString();
			Stop(id, move(name));
		}
		else if (info.AsMap().at("type"s).AsString() == "Map"s) {
			std::ostringstream ostr;
			picture::RenderVector(ostr, render);

			json::Dict mapa = { {"map"s, ostr.str()}, {"request_id"s, id} };
			req_answer_.push_back(mapa);
		}
	}
	json::Print(json::Document{ req_answer_ }, out);
}


void RequestHandler::Bus(int id, std::string&& bus) {

	auto [exists, info] = catalogue_.GetBusInfo(move(bus));
	if (exists) {
		json::Dict bus_info;

		bus_info.emplace("request_id"s, id);
		bus_info.emplace("curvature"s, info.curv);
		bus_info.emplace("route_length"s, info.length);
		bus_info.emplace("stop_count"s, info.stops);
		bus_info.emplace("unique_stop_count"s, info.uniq_stops);

		req_answer_.push_back( bus_info);
	}
	else {
		req_answer_.push_back( json::Dict{ { "request_id"s, id}, {"error_message"s, "not found"s} });
	}
}


void RequestHandler::Stop(int id, string&& stop) {

	auto [exists, buses] = catalogue_.GetStopInfo(move(stop));
	if (exists) {
		json::Array buses_json;
		std::sort(buses.begin(), buses.end());
		for (auto bus : buses) {
			buses_json.push_back(static_cast<string>(bus));
		}

		json::Dict stop_info{ {"buses"s, buses_json}, {"request_id"s, id} };

		req_answer_.push_back( stop_info);
	}
	else {
		req_answer_.push_back( json::Dict{ { "request_id"s, id}, {"error_message"s, "not found"s} });
	}
}