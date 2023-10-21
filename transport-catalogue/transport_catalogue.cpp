#include "transport_catalogue.h"
#include "geo.h"

#include <algorithm>
#include <set>

using namespace std;

using namespace transport_catalogue;
using namespace details;
using namespace types;

//добавляет остановку в в деку, 
// из деки адрес остановки добавляется в мапу
// функция возвращает предыдущий элемент, добавленный в деку, если он есть, иначе возвращает добавленный элемент
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



// добавляет автобус с его маршрутом следования в деку,
// из деки адрес элемента добавляется в мапу
// функция возвращает предыдущий элемент, добавленный в деку, если он есть, иначе возвращает добавленный элемент
Buses* TransportCatalogue::AddBus(string&& name, vector<Stops*> route, string&& type) {
	stat_buses_.push_back({ name, route, type });
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


std::tuple<bool, int, int, double, double> TransportCatalogue::GetBusInfo(std::string_view name) {
	auto bus = buses_.find(name);
	bool found = true;

	//такого автобуса нет
	if (bus == buses_.end()) {
		found = false;
		return make_tuple(found, 0, 0, 0, 0);
	}

	int routes = static_cast<int>(bus->second->route.size());
	vector<Stops*> stops = bus->second->route;
	set<Stops*> uniq;
	double length = 0;
	double curv = 0, lin_curv = 0;

	for (int i = 1; i < stops.size(); ++i) {
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

	if (bus->second->type == "linear"s) {
		length *= 2;
		routes = routes * 2 - 1;
		curv += lin_curv;
	}
	double trtr = curv;
	curv /= length;
	int uni = static_cast<int>(uniq.size());
	return make_tuple(found, routes, uni, trtr, curv);
}


std::tuple<bool, std::vector<std::string_view>> TransportCatalogue::GetStopInfo(std::string_view stop) {
	auto st = FindStop(stop);
	vector<string_view> buses;
	bool exists = true;

	//остановки нет
	if (st == nullptr) {
		exists = false;
		return make_tuple(exists, buses);
	}

	for (auto& [bus_name, bus] : buses_) {
		//автобус из базы проезжает через эту остановку
		if (find(bus->route.begin(), bus->route.end(), st) != bus->route.end()) {
			buses.push_back(bus_name);
		}
	}

	//остановка может существовать, но через нее не будут ездить автобусы
	return make_tuple(exists, buses);
}


double TransportCatalogue::GetDistanceInfo(PairStops stops) {

	//если между А-В и В-А разные расстояния
	auto found = distance_.find(stops);
	if (found != distance_.end()) {
		return distance_.at(stops);
	}

	//если А-В == В-А
	found = distance_.find({ stops.second, stops.first });
	if (found != distance_.end()) {
		return distance_.at({ stops.second, stops.first });
	}

	//остановок нет
	return 0;
}


//для тестов
std::vector<std::string> TransportCatalogue::GetAllBuses() {
	vector<std::string> all_buses;
	for (auto& [name, route] : buses_) {
		all_buses.push_back(string(name));
	}
	return all_buses;
}