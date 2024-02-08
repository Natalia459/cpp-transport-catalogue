#include "transport_router.h"

using namespace std;
using namespace router;

TransportRouter::TransportRouter(const transport_catalogue::TransportCatalogue& catal)
	: graph_(catal.GetStopsCount() * 2), catalogue_(catal)
{
}

void TransportRouter::SetRouterSettings(types::RouterSettings setts) {
	settings_ = setts;
}

void TransportRouter::FillGraph() {
	InitAllVertexes();
	for (const auto& bus : catalogue_.GetAllBuses()) {

		AddNewEdges(bus->route.begin(), bus->route.end(), bus->name);

		if (!bus->is_circle) {
			AddNewEdges(bus->route.rbegin(), bus->route.rend(), bus->name);
		}
	}
	std::unique_ptr<graph::Router<double>> r(new graph::Router<double>(graph_));
	router_ = std::move(r);
}


// pair<exists, mapa(total_time, items)> GetRouteInfo
// items = Wait / Bus
types::RouteInfo TransportRouter::GetRouteInfo(std::string_view from_stop, std::string_view to_stop) const {
	auto [from_id, to_id] = GetFromAndToVertexIds(from_stop, to_stop);
	bool exists = true;

	auto route = router_->BuildRoute(from_id, to_id);
	if (route == std::nullopt) {
		return { !exists, 0, {} };
	}
	std::vector<types::RouteProperties> items = GetItems(route.value().edges);

	double total_time = route.value().weight;
	return types::RouteInfo{ exists, total_time, std::move(items) };
}

void TransportRouter::InitAllVertexes() {
	graph::VertexId id = 0;
	for (const auto& stop : catalogue_.GetAllStops()) {
		graph::VertexId from_id = id;
		ids_for_stops_[id] = stop->name;
		wait_end_stops_[stop->name] = id++;


		graph::VertexId to_id = id;
		ids_for_stops_[id] = stop->name;
		start_stops_[stop->name] = id++;

		graph::Edge<double> new_edge{ from_id, to_id, settings_.wait_time * 1.0 };
		graph_.AddEdge(new_edge);
	}
}

RouteTypes TransportRouter::DefineRouteType(graph::VertexId from, graph::VertexId to) const { //учесть vertex_id !!!!!!!!!!!!!
	std::string_view from_stop = ids_for_stops_.at(from);
	std::string_view to_stop = ids_for_stops_.at(to);
	if (from_stop == to_stop) {
		return RouteTypes::WAIT;
	}

	bool from_exists = stops_for_buses_.find(from) != stops_for_buses_.end();
	bool to_exists = (from_exists) ? stops_for_buses_.at(from).find(to) != stops_for_buses_.at(from).end() : false;
	if (from_exists && to_exists) {
		return (!stops_for_buses_.at(from).at(to).empty()) ? RouteTypes::BUS : RouteTypes::DEFAULT;
	}
	return RouteTypes::BUS;
}

std::vector<types::RouteProperties> TransportRouter::GetItems(const std::vector<graph::EdgeId> edge_ids) const {
	std::vector<types::RouteProperties> result;
	result.reserve(edge_ids.size());

	for (const auto& edge_id : edge_ids) {
		types::RouteProperties prop;
		const auto& edge_info = graph_.GetEdge(edge_id);
		auto type = DefineRouteType(edge_info.from, edge_info.to);

		if (type == RouteTypes::BUS) {
			auto [bus_name, span_count, time] = *(stops_for_buses_.at(edge_info.from).at(edge_info.to).begin()); // at error
			prop.type = "Bus";
			prop.time = time;
			prop.bus_name = bus_name;
			prop.span_count = span_count;
		}
		else if (type == RouteTypes::WAIT) {
			prop.type = "Wait";
			prop.time = settings_.wait_time;
			prop.stop_name = ids_for_stops_.at(edge_info.from);
		}
		else {
			continue;
		}
		result.push_back(prop);
	}
	return result;
}

std::pair<graph::VertexId, graph::VertexId> TransportRouter::GetFromAndToVertexIds(std::string_view from_stop, std::string_view to_stop) const {
	graph::VertexId from_id = 0, to_id = 0;
	if (start_stops_.find(from_stop) != start_stops_.end()) {
		from_id = wait_end_stops_.at(from_stop);
	}
	if (wait_end_stops_.find(to_stop) != wait_end_stops_.end()) {
		to_id = wait_end_stops_.at(to_stop);
	}
	return std::make_pair(from_id, to_id);
}

