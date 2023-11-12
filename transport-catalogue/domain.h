#pragma once
#include <string>
#include <vector>


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
}
