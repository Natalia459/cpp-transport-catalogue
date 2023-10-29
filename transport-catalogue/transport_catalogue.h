#pragma once

#include <unordered_map>
#include <string_view>
#include <string>
#include <vector>
#include <deque>
#include <tuple>

namespace transport_catalogue {
	namespace types {
		struct Stops {
			std::string name;
			double lati;
			double longi;
		};

		struct Buses {
			std::string name;
			std::vector<Stops*> route;
			bool is_circle; // if not "circle" then "linear"
		};

		struct PairStops {
			Stops* first;
			Stops* second;

			bool operator==(const PairStops& other) const {
				return (first == other.first) && (second == other.second);
			}
		};

		struct StopsHasher {
			size_t operator()(const PairStops& stops) const {
				return stops_hasher_(stops.first) * 31 + stops_hasher_(stops.second) * 31 * 31;
			}
		private:
			std::hash<Stops*> stops_hasher_{};
		};

		struct BusInfo {
			int routes;
			int unic_routes;
			double length;
			double curv;
		};
	}

	class TransportCatalogue {
	private:
		std::deque<types::Stops> stat_stops_;
		std::deque<types::Buses> stat_buses_;

		std::unordered_map<std::string_view, types::Stops*> stops_;
		std::unordered_map<std::string_view, types::Buses*> buses_;

		std::unordered_map<types::PairStops, double, types::StopsHasher> distance_;

	public:

		types::Stops* AddStop(std::string& name, double lat, double lon);

		types::Stops* FindStop(std::string_view name);

		std::pair<bool, std::vector<std::string_view>> GetStopInfo(std::string_view name);

		types::Buses* AddBus(std::string&& name, std::vector<types::Stops*>&& stops, bool is_circle);

		types::Buses* FindBus(std::string_view name);

		std::pair<bool, types::BusInfo> GetBusInfo(std::string_view name);

		void AddDistance(types::PairStops stops, double dist);
		 
		double GetDistanceInfo(types::PairStops stops);

		std::vector<std::string> GetAllBuses();
	};
}