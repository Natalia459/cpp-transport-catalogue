#pragma once
#include "router.h"
#include "transport_catalogue.h"

#include <map>
#include <string_view>
#include <optional>
#include <unordered_set>
#include <functional>
#include <memory>

namespace router {

	enum class RouteTypes {
		WAIT,
		BUS,
		DEFAULT
	};

	class TransportRouter {
	public:
		using StopName = std::string_view;
		double epsilon = 10e-6;

		// конструктор
		TransportRouter(const transport_catalogue::TransportCatalogue& catal);

		// pair<exists, mapa(total_time, items)> GetRouteInfo
		// items = Wait / Bus
		types::RouteInfo GetRouteInfo(std::string_view from_stop, std::string_view to_stop) const;

		void SetRouterSettings(types::RouterSettings setts);

		void FillGraph();

	private:
		std::map<StopName, graph::VertexId> start_stops_; // начала поездок
		std::map<StopName, graph::VertexId> wait_end_stops_; // wait/концы поездок
		std::map<graph::VertexId, std::map<graph::VertexId, std::unordered_set<types::BusProperties, types::HashBusProperties>>> stops_for_buses_; // map< from_StopName -> map< to_StopName -> bus_name + span_count > >
		std::map<graph::VertexId, StopName> ids_for_stops_; // декодер: координаты(из, в) - остановка ожидание/конец/начало пути + тип

		std::unique_ptr<graph::Router<double>> router_;
		graph::DirectedWeightedGraph<double> graph_;
		types::RouterSettings settings_;

		const transport_catalogue::TransportCatalogue& catalogue_;

		void InitAllVertexes();

		// инициализация ребер графа временем между остановками
		template<typename RandomIt>
		void AddNewEdges(RandomIt first, RandomIt last, std::string_view name);

		template<typename RandomIt>
		double CountDistance(RandomIt from, RandomIt to, size_t span_count) const;

		RouteTypes DefineRouteType(graph::VertexId from, graph::VertexId to) const;

		std::vector<types::RouteProperties> GetItems(const std::vector<graph::EdgeId> edge_ids) const;

		std::pair<graph::VertexId, graph::VertexId> GetFromAndToVertexIds(std::string_view from_stop, std::string_view to_stop) const;

	};



	// инициализация ребер графа временем между остановками
	template<typename RandomIt>
	void TransportRouter::AddNewEdges(RandomIt first, RandomIt last, std::string_view name) {
		for (auto iter_from = first; iter_from != last; ++iter_from) {
			double prev_dist = 0;
			size_t span_count = 1;

			auto& stop_from = *iter_from;
			for (auto iter_to = iter_from + 1; iter_to != last; ++iter_to) {
				auto& stop_to = *iter_to;
				double dist = CountDistance(iter_from, iter_to, span_count);
				prev_dist += dist;

				graph::VertexId id_from = start_stops_.at(stop_from->name);
				graph::VertexId id_to = wait_end_stops_.at(stop_to->name);

				double time = 0;
				if (dist != 0) {	
					time = prev_dist * 60. / (settings_.bus_velocity * 1000.);
					graph::Edge<double> new_edge{ id_from, id_to, time };
					graph_.AddEdge(new_edge);
				}

				stops_for_buses_[id_from][id_to].insert(types::BusProperties{ name, span_count, time }); //по этому маршруту уже ездит 1 автобус, но может ездить еще 1
				++span_count;
			}
		}
	}

	template<typename RandomIt>
	double TransportRouter::CountDistance(RandomIt from, RandomIt to, size_t span_count) const {
		return catalogue_.GetDistanceInfo(types::PairStops{ *(from + span_count - 1), *to });
	}

}