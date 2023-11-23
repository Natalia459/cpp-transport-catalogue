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


void RequestHandler::GetRequest(std::ostream& out, const json::Node& requests, picture::Renderer render) {
	req_answer_.StartArray();
	for (const auto& info : requests.AsArray()) {
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
			RenderVector(ostr, render);
			
			json::Dict mapa = { {"map"s, ostr.str()}, {"request_id"s, id}};
			req_answer_.Value(mapa);
		}
	}
	req_answer_.EndArray();
	json::Print(json::Document{ req_answer_.Build() }, out);
}


void RequestHandler::Bus(int id, std::string&& bus) {

	auto [exists, info] = catalogue_.GetBusInfo(move(bus));
	if (exists) {
		req_answer_.StartDict().
			Key("request_id"s).Value(id).
			Key("curvature"s).Value(info.curv).
			Key("route_length"s).Value(info.length).
			Key("stop_count"s).Value(info.stops).
			Key("unique_stop_count"s).Value(info.uniq_stops).
			EndDict();
	}
	else {
		req_answer_.StartDict().
			Key("request_id"s).Value(id).
			Key("error_message"s).Value("not found"s).
			EndDict();
	}
}


void RequestHandler::Stop(int id, string&& stop) {

	auto [exists, buses] = catalogue_.GetStopInfo(move(stop));
	if (exists) {
		json::Builder buses_json;
		auto arr = buses_json.StartArray();

		std::sort(buses.begin(), buses.end());
		for (auto bus : buses) {
			arr.Value(static_cast<string>(bus));
		}
		buses_json.EndArray();

		req_answer_.StartDict().
			Key("buses"s).Value(buses_json.Build()).
			Key("request_id"s).Value(id).
			EndDict();
	}
	else {
		req_answer_.StartDict().
			Key("request_id"s).Value(id).
			Key("error_message"s).Value("not found"s).
			EndDict();
	}
}