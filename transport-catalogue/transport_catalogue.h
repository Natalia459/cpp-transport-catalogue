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
			std::string type; //"circle" or "linear"
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
	}

	class TransportCatalogue {
	private:
		std::deque<types::Stops> stat_stops_;
		std::deque<types::Buses> stat_buses_;

		std::unordered_map<std::string_view, types::Stops*> stops_;
		std::unordered_map<std::string_view, types::Buses*> buses_;

		std::unordered_map<types::PairStops, double, types::StopsHasher> distance_;

	public:

		types::Stops* AddStop(std::string&, double, double);

		types::Stops* FindStop(std::string_view);

		std::tuple<bool, std::vector<std::string_view>> GetStopInfo(std::string_view);


		types::Buses* AddBus(std::string&&, std::vector<types::Stops*>, std::string&& type = "circle");

		types::Buses* FindBus(std::string_view);

		std::tuple<bool, int, int, double, double> GetBusInfo(std::string_view);

		void AddDistance(types::PairStops, double);

		double GetDistanceInfo(types::PairStops stops);

		std::vector<std::string> GetAllBuses();
	};
}