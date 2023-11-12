#include "json_reader.h"
#include "request_handler.h"
#include "map_renderer.h"

#include <sstream>
#include <string>

#include <map>
#include <vector>

using namespace std;
using namespace transport_catalogue;
using namespace types;
using namespace input;
using namespace json;


//================================================================================================= INPUT


void transport_catalogue::Data(std::istream& in, std::ostream& out, TransportCatalogue& catal) {
	const Document doc = json::Load(in);
	json::Dict render_data;
	json::Array request_data;
	std::vector<types::Buses*> buses;

	for (const auto& [request_type, data] : doc.GetRoot().AsMap()) {

		if (request_type == "base_requests"s) {
			buses = move(AddData(data.AsArray(), catal));
		}
		else if (request_type == "stat_requests") {
			request_data = data.AsArray();
		}
		else if (request_type == "render_settings"s) {
			render_data = data.AsMap();
		}
	}

	if (render_data.empty() == false) {
		if (request_data.empty() == false) {
			output::GetRequest(out, request_data, picture::GetRenderSettings(render_data, buses), catal);
		}
	}
}

std::vector<types::Buses*> input::AddData(const Array& data, TransportCatalogue& catalogue) {
	vector<DistanceForStops> dists;
	vector<json::Dict> buses;

	for (const auto& info : data) {
		if (info.AsMap().at("type"s).AsString() == "Stop"s) {
			Stop(info.AsMap(), catalogue);
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
		Bus(bus, catalogue);
	}

	for (auto [first_stop, second_stop, dist] : dists) {
		Distances(move(first_stop), move(second_stop), dist, catalogue);
	}

	return catalogue.GetAllBuses();
}

void input::Stop(const json::Dict& stop, TransportCatalogue& catalogue) {

	string name = stop.at("name"s).AsString();
	double lat = stop.at("latitude").AsDouble();
	double lon = stop.at("longitude").AsDouble();

	catalogue.AddStop(name, lat, lon);
}


void input::Distances(std::string&& stop, std::string&& another_stop, double dist, TransportCatalogue& catalogue) {
	Stops* stop1 = catalogue.FindStop(stop);
	Stops* stop2 = catalogue.FindStop(another_stop);

	catalogue.AddDistance({ stop1, stop2 }, dist);
}


void input::Bus(const json::Dict& bus, TransportCatalogue& catalogue) {
	if (bus.empty()) {
		return;
	}

	string name = bus.at("name"s).AsString();
	json::Array stops = bus.at("stops").AsArray();
	bool is_round = bus.at("is_roundtrip").AsBool();

	std::vector<types::Stops*> stops_ptr;
	for (const auto& stop : stops) {
		stops_ptr.push_back(catalogue.FindStop(stop.AsString()));
	}
	catalogue.AddBus(move(name), move(stops_ptr), is_round);
}