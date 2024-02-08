#include "request_handler.h"

#include <map>
#include <vector>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <string_view>


using namespace std;

using namespace transport_catalogue;
using namespace types;


//================================================================================================= OUTPUT


void RequestHandler::GetRequest(std::ostream& out, const json::Node& requests) {
	req_answer_.StartArray();
	for (const auto& info : requests.AsArray()) {
		if (info.AsMap().empty()) {
			continue;
		}
		id_ = info.AsMap().at("id"s).AsInt();

		if (info.AsMap().at("type"s).AsString() == "Bus"s) {
			string name = info.AsMap().at("name"s).AsString();
			Bus(move(name));
		}
		else if (info.AsMap().at("type"s).AsString() == "Stop"s) {
			string name = info.AsMap().at("name"s).AsString();
			Stop(move(name));
		}
		else if (info.AsMap().at("type"s).AsString() == "Map"s) {
			//Render();
			std::ostringstream ostr;
			RenderVector(ostr, render_);

			json::Dict mapa = { {"map"s, ostr.str()}, {"request_id"s, id_} };
			req_answer_.Value(mapa);
		}
		else if (info.AsMap().at("type"s).AsString() == "Route"s) {
			std::string from_stop = info.AsMap().at("from"s).AsString();
			std::string to_stop = info.AsMap().at("to"s).AsString();
			Route(from_stop, to_stop);
		}
	}
	req_answer_.EndArray();
	json::Print(json::Document{ req_answer_.Build() }, out);
}


void RequestHandler::Bus(std::string&& bus) {

	auto [exists, info] = catalogue_.GetBusInfo(move(bus));
	if (exists) {
		req_answer_.StartDict().
			Key("request_id"s).Value(id_).
			Key("curvature"s).Value(info.curv).
			Key("route_length"s).Value(info.length).
			Key("stop_count"s).Value(info.stops).
			Key("unique_stop_count"s).Value(info.uniq_stops).
			EndDict();
	}
	else {
		req_answer_.StartDict().
			Key("request_id"s).Value(id_).
			Key("error_message"s).Value("not found"s).
			EndDict();
	}
}


void RequestHandler::Stop(string&& stop) {

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
			Key("request_id"s).Value(id_).
			EndDict();
	}
	else {
		req_answer_.StartDict().
			Key("request_id"s).Value(id_).
			Key("error_message"s).Value("not found"s).
			EndDict();
	}
}

void RequestHandler::Render() {
	//std::ostringstream ostr;
	//RenderVector(ostr, render_);

	//json::Dict mapa = { {"map"s, ostr.str()}, {"request_id"s, id_} };
	//req_answer_.Value(mapa);
	//req_answer_.StartDict().
	//	Key("request_id"s).Value(id_).
	//	Key("map"s).Value(ostr.str()).
	//	EndDict();
}

void RequestHandler::Route(std::string_view from_stop, std::string_view to_stop) {
	auto [exists, total_time, items] = router_.GetRouteInfo(from_stop, to_stop);

	json::Builder routes_json;
	if (exists) {
		auto arr = routes_json.StartArray();
		for (const auto& [type, time, span_count, bus_name, stop_name] : items) {
			json::Dict mapa;
			if (type == "Bus") {
				mapa = { {"type"s, type}, {"bus"s, static_cast<string>(bus_name.value())}, {"span_count"s, static_cast<int>(span_count.value())}, {"time"s, time} };

			}
			else if (type == "Wait") {
				mapa = { {"type"s, type}, {"stop_name"s, static_cast<string>(stop_name.value())}, {"time"s, time} };
			}
			arr.Value(mapa);
		}
		routes_json.EndArray();

		req_answer_.StartDict().
			Key("request_id"s).Value(id_).
			Key("total_time"s).Value(total_time).
			Key("items"s).Value(routes_json.Build()).
			EndDict();
	}
	else {
		req_answer_.StartDict().
			Key("request_id"s).Value(id_).
			Key("error_message"s).Value("not found"s).
			EndDict();
	}
}