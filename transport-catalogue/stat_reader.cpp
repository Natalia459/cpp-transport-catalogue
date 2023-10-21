#include "stat_reader.h"

#include <iostream>
#include <iomanip>
#include <algorithm>

using namespace std;
using namespace transport_catalogue;
using namespace output;


void output::Request(istream& fin, ostream& fout, TransportCatalogue& cat) {
	int amount;
	fin >> amount;

	string name;
	getline(fin, name);
	while (amount--) {
		getline(fin, name);
		if (name.empty()) {
			continue;
		}
		size_t n = name.find(' ');
		string type = name.substr(0, n++);
		name = name.substr(n);
		if (type == "Bus"s) {
			StopsForBus(fout, move(name), cat);
		}
		else {
			BusesToStop(fout, move(name), cat);
		}
	}
}


void output::StopsForBus(ostream& fout, string&& bus, TransportCatalogue& cat) {

	fout << "Bus "s << bus << ": ";
	auto [found, stops, uniq, length, curv] = cat.GetBusInfo(bus);

	if (found) {
		fout << stops << " stops on route, "
			<< uniq << " unique stops, "
			<< std::setprecision(6) << length << " route length, "
			<< std::setprecision(6) << curv << " curvature\n";
	}
	else {
		fout << "not found\n";
	}
}


void output::BusesToStop(std::ostream& fout, string&& stop, TransportCatalogue& cat) {

	fout << "Stop "s << stop << ": ";
	auto [exists, buses] = cat.GetStopInfo(stop);

	if (exists) {
		if (buses.empty()) {
			fout << "no buses\n";
			return;
		}
		sort(buses.begin(), buses.end());
		buses.erase(unique(buses.begin(), buses.end()), buses.end());

		fout << "buses";
		for (auto bus : buses) {
			fout << " " << bus;
		}
		fout << '\n';
	}
	else {
		fout << "not found\n";
	}
}