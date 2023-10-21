#include "tests.h"
#include "input_reader.h"
#include "stat_reader.h"
#include "log_duration.h"

#include <cassert>
#include <string>
#include <fstream>
#include <iostream>
#include <istream>

using namespace std;
using namespace transport_catalogue;
using namespace types;
using namespace input;
using namespace output;

double EPSILON = 0.000001;


//OK
void tests::Tests() {
	cout << "\nTESTS: " << endl;
	AddStop();
	FindStop();

	AddBus();
	FindBus();
	ReadData();
	ShowData();
	GetBusInfo();
	GetStopInfo();
	GetDistanceInfo();

	tsB_case2_input();
	tsC_case1_input();
	PatrCCase2Input();

	cout << endl;
	cout << "\nSPEED: ";
	speed::AddStop();
	speed::AddBus();
	speed::FindStop();
	speed::FindBus();
	speed::ReadData();
	speed::ShowData();
}


void tests::AddStop() {
	using namespace std::literals;
	{
		TransportCatalogue cat;
		double lat = 0.2222222;
		double lon = 0.4444444;
		string fir = "first"s;
		string sec = "second"s;

		auto first = cat.AddStop(fir, lat, lon);

		assert((first->name == "first"s));
		assert((first->lati == lat) && (first->longi == lon));

		++lat; ++lon;
		auto second = cat.AddStop(sec, lat, lon);

		assert((second->name == "second"s));
		assert((second->lati == lat) && (second->longi == lon));
	}
	cout << "AddStop() success" << endl;
}
//OK

void tests::FindStop() {
	{
		TransportCatalogue cat;
		double lat = 0.2222222;
		double lon = 0.4444444;

		for (int i = 1; i < 21; ++i) {
			string is = std::to_string(i);
			cat.AddStop(is, lat++, lon++);
		}

		lat = 0.2222222;
		lon = 0.4444444;
		for (int i = 1; i < 21; ++i) {
			std::string s = std::to_string(i);
			auto m = cat.FindStop(s);

			assert(m->name == s);
			assert((m->lati - lat < EPSILON) && (m->longi - lon < EPSILON));
			++lat; ++lon;
		}
	}
	cout << "FindStop() success" << endl;
}
//OK

void tests::AddBus() {
	using namespace std::literals;
	{
		TransportCatalogue cat;
		double lat = 0.2222222;
		double lon = 0.4444444;
		string fir = "1"s;
		auto m = cat.AddStop(fir, lat, lon);

		vector<Stops*> stops;
		for (int i = 2; i <= 5; ++i) {
			string is = std::to_string(i);
			auto m = cat.AddStop(is, lat++, lon++);
			stops.push_back(cat.FindStop(std::to_string(i)));
		}
		auto bus = cat.AddBus("1", stops);
		std::string name = "1";
		assert((bus->name == Buses{ name, stops }.name));
		assert((bus->route == Buses{ name, stops }.route));
	}
	cout << "AddBus() success" << endl;
}
//OK

void tests::FindBus() {
	using namespace std::literals;
	{
		TransportCatalogue cat;
		double lat = 0.2222222;
		double lon = 0.4444444;
		std::string fi = "first";
		std::string s = "second";
		string t = "third";
		string fo = "fourth";

		cat.AddStop(fi, lat++, lon++);
		cat.AddStop(s, lat++, lon++);

		vector<Stops*> stops1;
		stops1.push_back(cat.FindStop(fi));
		stops1.push_back(cat.FindStop(s));

		std::string name1 = "1";
		cat.AddBus("1", stops1);

		cat.AddStop(t, lat++, lon++);
		cat.AddStop(fo, lat++, lon++);

		vector<Stops*> stops2;
		stops2.push_back(cat.FindStop(t));
		stops2.push_back(cat.FindStop(fo));

		std::string name2 = "2";
		cat.AddBus("2", stops2);

		auto bus1 = cat.FindBus(name1);
		auto bus2 = cat.FindBus(name2);

		assert((bus1->name == Buses{ name1, stops1 }.name));
		assert((bus1->route == Buses{ name1, stops1 }.route));

		assert((bus2->name == Buses{ name2, stops2 }.name));
		assert((bus2->route == Buses{ name2, stops2 }.route));
	}
	cout << "FindBus() success" << endl;
}
//OK

void tests::ReadData() {
	TransportCatalogue cat;
	std::fstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\pt1\\tsA_case1_input.txt");

	if (fin) {
		Data(fin, cat);
	}
	cout << "ReadData() success" << endl;
}
//OK

void tests::ShowData() {
	TransportCatalogue cat;
	std::fstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\pt1\\tsA_case1_input.txt");

	if (fin) {
		Data(fin, cat);
	}
	std::fstream fout;
	fout.open("test1_output", 'w');

	vector<string> all_buses = move(cat.GetAllBuses());
	for (auto& bus : all_buses) {
		StopsForBus(fout, move(bus), cat);
	}

	cout << "ShowData() success" << endl;
}
//OK

void tests::GetBusInfo() {
	std::istringstream sin{
		"10\n"
		"Stop Tolstopaltsevo: 55.611087, 37.208290\n"
		"Stop Marushkino: 55.595884, 37.209755\n"
		"Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
		"Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
		"Stop Rasskazovka: 55.632761, 37.333324\n"
		"Stop Biryulyovo Zapadnoye: 55.574371, 37.651700\n"
		"Stop Biryusinka: 55.581065, 37.648390\n"
		"Stop Universam: 55.587655, 37.645687\n"
		"Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\n"
		"Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n"
		"3\n"
		"Bus 256\n"
		"Bus 750\n"
		"Bus 751\n"
	};

	std::ostringstream sout{
		"Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length\n"
		"Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length\n"
		"Bus 751: not found\n"
	};

	TransportCatalogue cat;
	Data(sin, cat);

	vector<string> all_buses = { "256"s, "750"s, "751"s };
	for (auto& bus : all_buses) {
		StopsForBus(cout, move(bus), cat);
	}

	cout << "GetBusInfo() success" << endl;
}
//OK

void tests::GetStopInfo() {
	std::istringstream sin{
		"13\n"
		"Stop Tolstopaltsevo : 55.611087, 37.20829\n"
		"Stop Marushkino : 55.595884, 37.209755\n"
		"Bus 256 : Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
		"Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
		"Stop Rasskazovka : 55.632761, 37.333324\n"
		"Stop Biryulyovo Zapadnoye : 55.574371, 37.6517\n"
		"Stop Biryusinka : 55.581065, 37.64839\n"
		"Stop Universam : 55.587655, 37.645687\n"
		"Stop Biryulyovo Tovarnaya : 55.592028, 37.653656\n"
		"Stop Biryulyovo Passazhirskaya : 55.580999, 37.659164\n"
		"Bus 828 : Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n"
		"Stop Rossoshanskaya ulitsa : 55.595579, 37.605757\n"
		"Stop Prazhskaya : 55.611678, 37.603831\n"
		"6\n"
		"Bus 256\n"
		"Bus 750\n"
		"Bus 751\n"
		"Stop Samara\n"
		"Stop Prazhskaya\n"
		"Stop Biryulyovo Zapadnoye\n"
	};

	std::istringstream sout{
		"Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length\n"
		"Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length\n"
		"Bus 751: not found\n"
		"Stop Samara: not found\n"
		"Stop Prazhskaya : no buses\n"
		"Stop Biryulyovo Zapadnoye : buses 256 828\n"
	};

	TransportCatalogue cat;
	Data(sin, cat);

	std::ostringstream ssut;

	Request(sout, cout, cat);

	cout << "GetBusInfo() success" << endl;
}
//OK

void tests::tsB_case1_input() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\pt2\\tsB_case1_input.txt");

	if (fin) {
		Data(fin, cat);
	}

	std::fstream fout;
	fout.open("tsB_case1_output", 'w');

	LOG_DURATION_STREAM("tsB_case1_input", std::cerr);
	if (fin && fout) {
		Request(fin, fout, cat);
	}

	cout << "tsB_case1_input success" << endl;
}
//OK

void tests::tsB_case2_input() {
	TransportCatalogue cat;
	std::fstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\pt2\\tsB_case2_input.txt");

	if (fin) {
		Data(fin, cat);
	}

	std::fstream fout;
	fout.open("tsB_case2_output", 'w');

	LOG_DURATION_STREAM("tsB_case2_input", std::cerr);
	if (fin && fout) {
		Request(fin, fout, cat);
	}

	cout << "tsB_case2_input success" << endl;
}
//OK

void tests::GetDistanceInfo() {
	std::istringstream sin{
	"13\n"
	"Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino\n"
	"Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka, 100m to Marushkino\n"
	"Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoy\n"
	"Bus 750: Tolstopaltsevo - Marushkino - Marushkino - Rasskazovka\n"
	"Stop Rasskazovka: 55.632761, 37.333324, 9500m to Marushkino\n"
	"Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam\n"
	"Stop Biryusinka: 55.581065, 37.64839, 750m to Universam\n"
	"Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya\n"
	"Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya\n"
	"Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye\n"
	"Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n"
	"Stop Rossoshanskaya ulitsa: 55.595579, 37.605757\n"
	"Stop Prazhskaya: 55.611678, 37.603831\n"
	"6\n"
	"Bus 256\n"
	"Bus 750\n"
	"Bus 751\n"
	"Stop Samara\n"
	"Stop Prazhskaya\n"
	"Stop Biryulyovo Zapadnoye\n"
	};

	std::istringstream sout{
		"Bus 256: 6 stops on route, 5 unique stops, 5950 route length, 1.36124 curvature\n"
		"Bus 750: 7 stops on route, 3 unique stops, 27400 route length, 1.30853 curvature\n"
		"Bus 751: not found\n"
		"Stop Samara: not found\n"
		"Stop Prazhskaya : no buses\n"
		"Stop Biryulyovo Zapadnoye : buses 256 828\n"
	};

	TransportCatalogue cat;
	Data(sin, cat);

	std::ostringstream ssut;

	Request(sout, cout, cat);

	cout << "GetDistanceInfo() success" << endl;

}
//OK

void tests::tsC_case1_input() {
	TransportCatalogue cat;
	std::fstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\pt3\\tsC_case1_input.txt");

	if (fin) {
		Data(fin, cat);
	}

	std::fstream fout;
	fout.open("tsC_output", 'w');

	LOG_DURATION_STREAM("tsC_case1_input", std::cerr);
	if (fin && fout) {
		Request(fin, fout, cat);
	}

	cout << "tsC_case1_input success" << endl;
}
//OK

void tests::PatrCCase2Input() {
	TransportCatalogue cat;
	std::fstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\part_c_case\\PatrCCase2Input.txt");

	if (fin) {
		Data(fin, cat);
	}

	std::fstream fout;
	fout.open("tsC_output", 'w');

	LOG_DURATION_STREAM("PatrC_output", std::cerr);
	if (fin && fout) {
		Request(fin, fout, cat);
	}

	cout << "PatrCCase2Input success" << endl;
}
//OK



using namespace tests;

void speed::AddStop() {
	{
		TransportCatalogue cat;
		double lat = 0.2222222;
		double lon = 0.4444444;

		LOG_DURATION_STREAM("AddStop", std::cerr);
		for (int i = 0; i < 500; ++i) {
			string is = std::to_string(i);
			cat.AddStop(is, lat, lon);
			++lat; ++lon;
		}
	}
	cout << "AddStop() success" << endl;
}
//OK

void speed::AddBus() {
	using namespace std::literals;
	{
		TransportCatalogue cat;
		double lat = 0.2222222;
		double lon = 0.4444444;
		string fir = "1"s;
		auto m = cat.AddStop(fir, lat, lon);

		vector<Stops*> stops;

		LOG_DURATION_STREAM("AddBus", std::cerr);
		for (int a = 1; a < 100; ++a) {
			stops.clear();

			for (int i = a * 2; i <= a * 20; ++i) {
				string is = std::to_string(i);
				auto m = cat.AddStop(is, lat++, lon++);
				stops.push_back(cat.FindStop(std::to_string(i)));
			}
			auto bus = cat.AddBus(std::to_string(a), stops);
		}
	}
	cout << "AddBus() success" << endl;
}
//OK

void speed::FindStop() {
	{
		TransportCatalogue cat;
		double lat = 0.2222222;
		double lon = 0.4444444;

		for (int i = 0; i < 250; ++i) {
			string is = std::to_string(i);
			cat.AddStop(is, lat, lon);
			++lat; ++lon;
		}

		lat = 0.2222222;
		lon = 0.4444444;

		LOG_DURATION_STREAM("FindStop", std::cerr);
		for (int i = 0; i < 25'000; ++i) {
			cat.FindStop(std::to_string(i));
		}
	}
	cout << "FindStop() success" << endl;
}
//OK

void speed::FindBus() {
	{
		TransportCatalogue cat;
		double lat = 0.2222222;
		double lon = 0.4444444;
		string fir = "1"s;
		auto m = cat.AddStop(fir, lat, lon);

		vector<Stops*> stops;
		for (int a = 1; a < 100; ++a) {
			stops.clear();

			for (int i = a * 2; i <= a * 20; ++i) {
				string is = std::to_string(i);
				auto m = cat.AddStop(is, lat++, lon++);
				stops.push_back(cat.FindStop(std::to_string(i)));
			}
			auto bus = cat.AddBus(std::to_string(a), stops);
		}

		LOG_DURATION_STREAM("AddBus", std::cerr);
		for (int a = 1; a < 100; ++a) {
			cat.FindBus(std::to_string(a));
		}
	}
	cout << "FindBus() success" << endl;
}
//OK

void speed::ReadData() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\pt1\\tsA_case2_input.txt");

	if (fin) {
		LOG_DURATION_STREAM("ReadData", std::cerr);
		Data(fin, cat);
	}

	cout << "ReadData() success" << endl;
}
//OK

void speed::ShowData() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\pt1\\tsA_case2_input.txt");

	if (fin) {
		Data(fin, cat);
	}

	std::fstream fout;
	fout.open("tsA_case2_output", 'w');

	vector<string> all_buses = move(cat.GetAllBuses());
	LOG_DURATION_STREAM("ShowData", std::cerr);
	for (auto& bus : all_buses) {
		StopsForBus(fout, move(bus), cat);
	}

	cout << "ShowData() success" << endl;
}
//OK