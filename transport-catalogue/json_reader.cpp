#include "json_reader.h"
#include "request_handler.h"
#include "map_renderer.h"
#include "json_builder.h"

#include <sstream>
#include <string>

#include <map>
#include <vector>

using namespace std;
using namespace transport_catalogue;
using namespace types;
using namespace json;


//================================================================================================= INPUT


void JSONReader::ProcessData(std::istream& in, std::ostream& out) {

	const Document doc = json::Load(in);
	json::Builder render_data;
	json::Builder request_data;
	std::vector<types::Buses*> buses;
	types::RouterSettings setts;

	for (const auto& [request_type, data] : doc.GetRoot().AsMap()) {

		if (request_type == "base_requests"s) {
			buses = move(AddData(data.AsArray()));
		}
		else if (request_type == "stat_requests") {
			request_data.Value(data.AsArray());
		}
		else if (request_type == "render_settings"s) {
			render_data.Value(data.AsMap());
		}
		else if (request_type == "routing_settings"s) {
			setts = GetRouterSettings(data.AsMap());
		}
	}
	router::TransportRouter router(catalogue_);
	router.SetRouterSettings(setts);
	router.FillGraph();

	if (render_data.Build().AsMap().empty() == false) {
		if (request_data.Build().AsArray().empty() == false) {
			RequestHandler handler(catalogue_, picture::GetRenderSettings(render_data.Build(), buses), router);
			handler.GetRequest(out, request_data.Build());
		}
	}
}

std::vector<types::Buses*> JSONReader::AddData(const Array& data) {

	vector<DistanceForStops> dists;
	vector<json::Dict> buses;

	for (const auto& info : data) {
		if (info.AsMap().at("type"s).AsString() == "Stop"s) {
			Stop(info.AsMap());
			auto stop = info.AsMap();
			Dict road_dists = stop.at("road_distances"s).AsMap();
			string first_stop = stop.at("name"s).AsString();

			for (auto [second_stop, dist] : road_dists) {
				dists.push_back({ first_stop, second_stop, dist.AsDouble() });
			}
		}
		else if (info.AsMap().at("type"s).AsString() == "Bus"s) {
			buses.push_back(info.AsMap());
		}
	}

	for (auto bus : buses) {
		Bus(bus);
	}

	for (auto [first_stop, second_stop, dist] : dists) {
		Distances(move(first_stop), move(second_stop), dist);
	}

	return catalogue_.GetAllBuses();
}

void JSONReader::Stop(const json::Dict& stop) {

	string name = stop.at("name"s).AsString();
	double lat = stop.at("latitude").AsDouble();
	double lon = stop.at("longitude").AsDouble();

	catalogue_.AddStop(name, lat, lon);
}


void JSONReader::Distances(std::string&& stop, std::string&& another_stop, double dist) {

	Stops* stop1 = catalogue_.FindStop(stop);
	Stops* stop2 = catalogue_.FindStop(another_stop);

	catalogue_.AddDistance({ stop1, stop2 }, dist);
}


void JSONReader::Bus(const json::Dict& bus) {
	if (bus.empty()) {
		return;
	}

	string name = bus.at("name"s).AsString();
	json::Array stops = bus.at("stops").AsArray();
	bool is_round = bus.at("is_roundtrip").AsBool();

	std::vector<types::Stops*> stops_ptr;
	for (const auto& stop : stops) {
		stops_ptr.push_back(catalogue_.FindStop(stop.AsString()));
	}
	catalogue_.AddBus(move(name), move(stops_ptr), is_round);
}

types::RouterSettings JSONReader::GetRouterSettings(const json::Dict& setts) const {
	double velocity = setts.at("bus_velocity").AsDouble();
	int time = setts.at("bus_wait_time").AsInt();
	return { velocity, time };
}