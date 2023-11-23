#include "transport_catalogue.h"
#include "geo.h"

#include <algorithm>
#include <set>

using namespace std;

using namespace transport_catalogue;
using namespace details;
using namespace types;

// adds a stop to deque,
// from the deque the address of the stop is added to the map
// the function returns the added stop
Stops* TransportCatalogue::AddStop(string& name, double lat, double lon) {
	stat_stops_.push_back({ name, lat, lon });
	stops_[stat_stops_.back().name] = &stat_stops_.back();

	return &*(stat_stops_.end() - 1);
}


Stops* TransportCatalogue::FindStop(string_view name) {
	auto stop = stops_.find(name);
	if (stop == stops_.end()) {
		return nullptr;
	}
	return *&stop->second;
}



// adds a bus with its route to deque,
// from the deque the address of the bus is added to the map
// the function returns the added bus
Buses* TransportCatalogue::AddBus(string&& name, vector<Stops*>&& stops, bool is_circle) {
	stat_buses_.push_back({ name, stops, is_circle });
	buses_[stat_buses_.back().name] = &stat_buses_.back();

	return &*(stat_buses_.end() - 1);
}


Buses* TransportCatalogue::FindBus(std::string_view name) {
	auto bus = buses_.find(name);
	if (bus == buses_.end()) {
		return nullptr;
	}
	return &*bus->second;
}


void TransportCatalogue::AddDistance(PairStops stops, double dist) {
	distance_[stops] = dist;
}


std::pair<bool, BusInfo> TransportCatalogue::GetBusInfo(std::string_view name) {
	auto bus = buses_.find(name);
	bool found = true;

	//there is no such bus
	if (bus == buses_.end()) {
		found = false;
		return make_pair(found, BusInfo{ 0, 0, 0, 0 });
	}

	int routes = static_cast<int>(bus->second->route.size());
	vector<Stops*> stops = bus->second->route;
	set<Stops*> uniq;
	double length = 0;
	double curv = 0, lin_curv = 0;

	for (size_t i = 1; i < stops.size(); ++i) {
		Coordinates coo{ stops[i - 1]->lati, stops[i - 1]->longi };
		Coordinates ecoo{ stops[i]->lati, stops[i]->longi };

		double len = ComputeDistance(coo, ecoo);

		double dist = GetDistanceInfo({ stops[i - 1], stops[i] });
		double lin_dist = GetDistanceInfo({ stops[i], stops[i - 1] });

		length += len;
		curv += dist;
		lin_curv += lin_dist;

		uniq.insert(stops[i - 1]);
	}
	uniq.insert(stops.back());

	//if route is linear
	if (bus->second->is_circle == false) {
		length *= 2;
		routes = routes * 2 - 1;
		curv += lin_curv;
	}
	double trtr = curv;
	curv /= length;
	int uni = static_cast<int>(uniq.size());

	return make_pair(found, BusInfo{ routes, uni, trtr, curv });
}


std::pair<bool, std::vector<std::string_view>> TransportCatalogue::GetStopInfo(std::string_view stop) {
	auto st = FindStop(stop);
	vector<string_view> buses;
	bool exists = true;

	//no such stop
	if (st == nullptr) {
		exists = false;
		return make_pair(exists, buses);
	}

	for (auto& [bus_name, bus] : buses_) {
		//the bus from the base passes through this stop
		if (find(bus->route.begin(), bus->route.end(), st) != bus->route.end()) {
			buses.push_back(bus_name);
		}
	}

	//the stop may exist, but buses will not travel through it
	return make_pair(exists, buses);
}


double TransportCatalogue::GetDistanceInfo(PairStops stops) {

	//if between a-b and b-a are different distances
	auto found = distance_.find(stops);
	if (found != distance_.end()) {
		return distance_.at(stops);
	}

	//id a-b == b-a
	found = distance_.find({ stops.second, stops.first });
	if (found != distance_.end()) {
		return distance_.at({ stops.second, stops.first });
	}

	//no stops
	return 0;
}


std::vector<types::Buses*> TransportCatalogue::GetAllBuses() const {
	//vector<std::string> all_buses;
	//all_buses.reserve(buses_.size());

	//for (auto& [name, route] : buses_) {
	//	if (route->route.empty() == false) {
	//		all_buses.push_back(static_cast<string>(name));
	//	}
	//}

	//std::sort(all_buses.begin(), all_buses.end());
	std::vector<types::Buses*> buses;
	for (const auto& [str_view, bus] : buses_) {
		buses.push_back(bus);
	}
	std::sort(buses.begin(), buses.end(), [](const auto& lhs, const auto& rhs)
		{
			return lhs->name < rhs->name;
		});

	return buses;
}