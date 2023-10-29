#include "transport_catalogue.h"
#include "input_reader.h"

#include <map>
#include <vector>

using namespace std;

using namespace transport_catalogue;
using namespace input;
using namespace types;

void input::Data(istream& in, TransportCatalogue& catal) {
	int amount;
	in >> amount;
	vector<string> buses;
	vector<DistanceForStops> stop_distances;

	std::string line;
	std::getline(in, line);
	while (amount--) {
		std::getline(in, line);

		if (line.empty()) {
			continue;
		}
		size_t space = line.find_first_of(' ');
		string type = line.substr(0, space++);
		line = line.substr(space);

		if (type == "Stop"s) {
			Stop(catal, move(line), stop_distances);
		}
		else {
			buses.push_back(move(line));
		}
	}

	for (auto& line : buses) {
		Bus(catal, move(line));
	}

	for (auto& stop : stop_distances) {
		Distances(catal, move(stop.first), move(stop.second), stop.dist);
	}
}


void input::Stop(TransportCatalogue& catal, std::string&& line, vector<DistanceForStops>& stop_distances) {
	size_t colon = line.find(':');
	string stop_name = line.substr(0, colon);
	size_t space = colon + 2;

	size_t comma = line.find(',');
	double lat = stod(line.substr(space, comma++));
	++comma;

	line = line.substr(comma);
	comma = line.find(',');
	double lon = stod(line.substr(0, comma++));

	catal.AddStop(stop_name, lat, lon);

	line = line.substr(comma + 1);

	comma = line.find(',');
	while (comma != line.npos) {
		space = line.find(' ');
		double dist = stod(line.substr(0, space - 1));
		space += 4;
		string another_stop_name = line.substr(space, comma - space);

		stop_distances.push_back({ stop_name, another_stop_name, dist });
		line = line.substr(comma + 2);
		comma = line.find(',');
	}
	space = line.find(' ');
	double dist = stod(line.substr(0, space - 1));
	space += 4;
	string another_stop_name = line.substr(space, comma - space);

	stop_distances.push_back({ stop_name, another_stop_name, dist });
}


void input::Distances(TransportCatalogue& catal, std::string&& stop, std::string&& another_stop, double dist) {
	Stops* stop1 = catal.FindStop(stop);
	Stops* stop2 = catal.FindStop(another_stop);

	catal.AddDistance({ stop1, stop2 }, dist);
}


void input::Bus(TransportCatalogue& catal, std::string&& line) {
	size_t colon = line.find(':');
	string name = line.substr(0, colon);

	line = line.substr(colon + 2);

	size_t dash = line.find_first_of('-');
	char sign = (dash != line.npos) ? '-' : '>';

	vector<Stops*> stops;
	size_t divider = line.find_first_of(sign);

	while (divider != line.npos) {
		string stop = line.substr(0, divider - 1);
		auto pointer = catal.FindStop(stop);
		stops.push_back(pointer);

		line = line.substr(divider + 2);
		divider = line.find_first_of(sign);
	}
	string stop = line.substr(0, divider - 1);
	auto pointer = catal.FindStop(stop);
	stops.push_back(pointer);

	//if route is linear
	if (sign == '-') {
		catal.AddBus(move(name), move(stops), /*is_circle =*/ false);
	}
	else {
		catal.AddBus(move(name), move(stops), /*is_circle =*/ true);
	}
}