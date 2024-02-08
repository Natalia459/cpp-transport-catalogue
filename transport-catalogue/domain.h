#pragma once
#include <string>
#include <vector>
#include <optional>
#include <string_view>
#include <functional>


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
		int stops;
		int uniq_stops;
		double length;
		double curv;
	};

	struct BusProperties {
		std::string_view name;
		size_t span_count;
		double time;

		bool operator<(const BusProperties& other) const {
			return name < other.name;
		}
		bool operator==(const BusProperties& other) const {
			return name == other.name && span_count == other.span_count;
		}
	};

	struct HashBusProperties{
		
		size_t operator()(const BusProperties& x) const{
			return hasher(x.name);
		}
		//template <typename Rhs, typename Lhs>
		//bool operator()(const Rhs& a, const Lhs& b) const {
		//	return a == b;
		//}

		std::hash<std::string_view> hasher;
	};



	struct RouterSettings {
		double bus_velocity = 0;
		int wait_time = 0;
	};

	struct RouteProperties {
		std::string type;
		double time;
		std::optional<size_t> span_count = std::nullopt;
		std::optional<std::string_view> bus_name = std::nullopt;
		std::optional<std::string_view> stop_name = std::nullopt;
	};

	struct RouteInfo {
		bool exists;
		double total_time;
		std::vector<RouteProperties> props;
	};

}
