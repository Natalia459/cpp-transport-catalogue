#include "tests.h"
#include "request_handler.h"
#include "json_reader.h"
#include "log_duration.h"

#include <cassert>
#include <string>
#include <fstream>
#include <iostream>
#include <istream>

using namespace std;
using namespace transport_catalogue;
using namespace types;
using namespace tests;

double EPSILON = 0.000001;


////OK
//void tests::Tests() {
//	cout << "\nTESTS: " << endl;
//	AddStop();
//	FindStop();
//
//	AddBus();
//	FindBus();
//	ReadData();
//	ShowData();
//	GetBusInfo();
//	GetStopInfo();
//	GetDistanceInfo();
//
//	tsB_case2_input();
//	tsC_case1_input();
//	PatrCCase2Input();
//
//	cout << endl;
//	cout << "\nSPEED: ";
//	speed::AddStop();
//	speed::AddBus();
//	speed::FindStop();
//	speed::FindBus();
//	speed::ReadData();
//	speed::ShowData();
//}
//
//
//
//
//void tests::AddStop() {
//	using namespace std::literals;
//	{
//		TransportCatalogue cat;
//		double lat = 0.2222222;
//		double lon = 0.4444444;
//		string fir = "first"s;
//		string sec = "second"s;
//
//		auto first = cat.AddStop(fir, lat, lon);
//
//		assert((first->name == "first"s));
//		assert((first->lati == lat) && (first->longi == lon));
//
//		++lat; ++lon;
//		auto second = cat.AddStop(sec, lat, lon);
//
//		assert((second->name == "second"s));
//		assert((second->lati == lat) && (second->longi == lon));
//	}
//	cout << "AddStop() success" << endl;
//}
////OK
//
//void tests::FindStop() {
//	{
//		TransportCatalogue cat;
//		double lat = 0.2222222;
//		double lon = 0.4444444;
//
//		for (int i = 1; i < 21; ++i) {
//			string is = std::to_string(i);
//			cat.AddStop(is, lat++, lon++);
//		}
//
//		lat = 0.2222222;
//		lon = 0.4444444;
//		for (int i = 1; i < 21; ++i) {
//			std::string s = std::to_string(i);
//			auto m = cat.FindStop(s);
//
//			assert(m->name == s);
//			assert((m->lati - lat < EPSILON) && (m->longi - lon < EPSILON));
//			++lat; ++lon;
//		}
//	}
//	cout << "FindStop() success" << endl;
//}
////OK
//
//void tests::AddBus() {
//	using namespace std::literals;
//	{
//		TransportCatalogue cat;
//		double lat = 0.2222222;
//		double lon = 0.4444444;
//		string fir = "1"s;
//		auto m = cat.AddStop(fir, lat, lon);
//
//		vector<Stops*> stops;
//		for (int i = 2; i <= 5; ++i) {
//			string is = std::to_string(i);
//			auto m = cat.AddStop(is, lat++, lon++);
//			stops.push_back(cat.FindStop(std::to_string(i)));
//		}
//		auto bus = cat.AddBus("1", move(stops));
//		std::string name = "1";
//		assert((bus->name == Buses{ name, stops }.name));
//		assert((bus->route == Buses{ name, stops }.route));
//	}
//	cout << "AddBus() success" << endl;
//}
////OK
//
//void tests::FindBus() {
//	using namespace std::literals;
//	{
//		TransportCatalogue cat;
//		double lat = 0.2222222;
//		double lon = 0.4444444;
//		std::string fi = "first";
//		std::string s = "second";
//		string t = "third";
//		string fo = "fourth";
//
//		cat.AddStop(fi, lat++, lon++);
//		cat.AddStop(s, lat++, lon++);
//
//		vector<Stops*> stops1;
//		stops1.push_back(cat.FindStop(fi));
//		stops1.push_back(cat.FindStop(s));
//		vector<Stops*> stops1_copy = stops1;
//
//		std::string name1 = "1";
//		cat.AddBus("1", move(stops1));
//
//		cat.AddStop(t, lat++, lon++);
//		cat.AddStop(fo, lat++, lon++);
//
//		vector<Stops*> stops2;
//		stops2.push_back(cat.FindStop(t));
//		stops2.push_back(cat.FindStop(fo));
//		vector<Stops*> stops2_copy = stops2;
//
//
//		std::string name2 = "2";
//		cat.AddBus("2", move(stops2));
//
//		auto bus1 = cat.FindBus(name1);
//		auto bus2 = cat.FindBus(name2);
//
//		assert((bus1->name == Buses{ name1, stops1_copy }.name));
//		assert((bus1->route == Buses{ name1, stops1_copy }.route));
//
//		assert((bus2->name == Buses{ name2, stops2_copy }.name));
//		assert((bus2->route == Buses{ name2, stops2_copy }.route));
//	}
//	cout << "FindBus() success" << endl;
//}
////OK
//
//void tests::ReadData() {
//	TransportCatalogue cat;
//	std::fstream fin;
//	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\pt1\\tsA_case1_input.txt");
//
//	if (fin) {
//		Data(fin, cat);
//	}
//	cout << "ReadData() success" << endl;
//}
////OK
//
//void tests::ShowData() {
//	TransportCatalogue cat;
//	std::fstream fin;
//	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\pt1\\tsA_case1_input.txt");
//
//	if (fin) {
//		Data(fin, cat);
//	}
//	std::fstream fout;
//	fout.open("test1_output", 'w');
//
//	vector<string> all_buses = move(cat.GetAllBuses());
//	for (auto& bus : all_buses) {
//		Bus(fout, move(bus), cat);
//	}
//
//	cout << "ShowData() success" << endl;
//}
////OK
//
//void tests::GetBusInfo() {
//	std::istringstream sin{
//		"10\n"
//		"Stop Tolstopaltsevo: 55.611087, 37.208290\n"
//		"Stop Marushkino: 55.595884, 37.209755\n"
//		"Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
//		"Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
//		"Stop Rasskazovka: 55.632761, 37.333324\n"
//		"Stop Biryulyovo Zapadnoye: 55.574371, 37.651700\n"
//		"Stop Biryusinka: 55.581065, 37.648390\n"
//		"Stop Universam: 55.587655, 37.645687\n"
//		"Stop Biryulyovo Tovarnaya: 55.592028, 37.653656\n"
//		"Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164\n"
//		"3\n"
//		"Bus 256\n"
//		"Bus 750\n"
//		"Bus 751\n"
//	};
//
//	std::ostringstream sout{
//		"Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length\n"
//		"Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length\n"
//		"Bus 751: not found\n"
//	};
//
//	TransportCatalogue cat;
//	Data(sin, cat);
//
//	vector<string> all_buses = { "256"s, "750"s, "751"s };
//	for (auto& bus : all_buses) {
//		Bus(cout, move(bus), cat);
//	}
//
//	cout << "GetBusInfo() success" << endl;
//}
////OK
//
//void tests::GetStopInfo() {
//	std::istringstream sin{
//		"13\n"
//		"Stop Tolstopaltsevo : 55.611087, 37.20829\n"
//		"Stop Marushkino : 55.595884, 37.209755\n"
//		"Bus 256 : Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye\n"
//		"Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka\n"
//		"Stop Rasskazovka : 55.632761, 37.333324\n"
//		"Stop Biryulyovo Zapadnoye : 55.574371, 37.6517\n"
//		"Stop Biryusinka : 55.581065, 37.64839\n"
//		"Stop Universam : 55.587655, 37.645687\n"
//		"Stop Biryulyovo Tovarnaya : 55.592028, 37.653656\n"
//		"Stop Biryulyovo Passazhirskaya : 55.580999, 37.659164\n"
//		"Bus 828 : Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n"
//		"Stop Rossoshanskaya ulitsa : 55.595579, 37.605757\n"
//		"Stop Prazhskaya : 55.611678, 37.603831\n"
//		"6\n"
//		"Bus 256\n"
//		"Bus 750\n"
//		"Bus 751\n"
//		"Stop Samara\n"
//		"Stop Prazhskaya\n"
//		"Stop Biryulyovo Zapadnoye\n"
//	};
//
//	std::istringstream sout{
//		"Bus 256: 6 stops on route, 5 unique stops, 4371.02 route length\n"
//		"Bus 750: 5 stops on route, 3 unique stops, 20939.5 route length\n"
//		"Bus 751: not found\n"
//		"Stop Samara: not found\n"
//		"Stop Prazhskaya : no buses\n"
//		"Stop Biryulyovo Zapadnoye : buses 256 828\n"
//	};
//
//	TransportCatalogue cat;
//	Data(sin, cat);
//
//	std::ostringstream ssut;
//
//	GetRequest(sout, cout, cat);
//
//	cout << "GetBusInfo() success" << endl;
//}
////OK
//
//void tests::tsB_case1_input() {
//	TransportCatalogue cat;
//	std::ifstream fin;
//	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\pt2\\tsB_case1_input.txt");
//
//	if (fin) {
//		Data(fin, cat);
//	}
//
//	std::fstream fout;
//	fout.open("tsB_case1_output", 'w');
//
//	LOG_DURATION_STREAM("tsB_case1_input", std::cerr);
//	if (fin && fout) {
//		GetRequest(fin, fout, cat);
//	}
//
//	cout << "tsB_case1_input success" << endl;
//}
////OK
//
//void tests::tsB_case2_input() {
//	TransportCatalogue cat;
//	std::fstream fin;
//	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\pt2\\tsB_case2_input.txt");
//
//	if (fin) {
//		Data(fin, cat);
//	}
//
//	std::fstream fout;
//	fout.open("tsB_case2_output", 'w');
//
//	LOG_DURATION_STREAM("tsB_case2_input", std::cerr);
//	if (fin && fout) {
//		GetRequest(fin, fout, cat);
//	}
//
//	cout << "tsB_case2_input success" << endl;
//}
////OK
//
//void tests::GetDistanceInfo() {
//	std::istringstream sin{
//	"13\n"
//	"Stop Tolstopaltsevo: 55.611087, 37.20829, 3900m to Marushkino\n"
//	"Stop Marushkino: 55.595884, 37.209755, 9900m to Rasskazovka, 100m to Marushkino\n"
//	"Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoy\n"
//	"Bus 750: Tolstopaltsevo - Marushkino - Marushkino - Rasskazovka\n"
//	"Stop Rasskazovka: 55.632761, 37.333324, 9500m to Marushkino\n"
//	"Stop Biryulyovo Zapadnoye: 55.574371, 37.6517, 7500m to Rossoshanskaya ulitsa, 1800m to Biryusinka, 2400m to Universam\n"
//	"Stop Biryusinka: 55.581065, 37.64839, 750m to Universam\n"
//	"Stop Universam: 55.587655, 37.645687, 5600m to Rossoshanskaya ulitsa, 900m to Biryulyovo Tovarnaya\n"
//	"Stop Biryulyovo Tovarnaya: 55.592028, 37.653656, 1300m to Biryulyovo Passazhirskaya\n"
//	"Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164, 1200m to Biryulyovo Zapadnoye\n"
//	"Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye\n"
//	"Stop Rossoshanskaya ulitsa: 55.595579, 37.605757\n"
//	"Stop Prazhskaya: 55.611678, 37.603831\n"
//	"6\n"
//	"Bus 256\n"
//	"Bus 750\n"
//	"Bus 751\n"
//	"Stop Samara\n"
//	"Stop Prazhskaya\n"
//	"Stop Biryulyovo Zapadnoye\n"
//	};
//
//	std::istringstream sout{
//		"Bus 256: 6 stops on route, 5 unique stops, 5950 route length, 1.36124 curvature\n"
//		"Bus 750: 7 stops on route, 3 unique stops, 27400 route length, 1.30853 curvature\n"
//		"Bus 751: not found\n"
//		"Stop Samara: not found\n"
//		"Stop Prazhskaya : no buses\n"
//		"Stop Biryulyovo Zapadnoye : buses 256 828\n"
//	};
//
//	TransportCatalogue cat;
//	Data(sin, cat);
//
//	std::ostringstream ssut;
//
//	GetRequest(sout, cout, cat);
//
//	cout << "GetDistanceInfo() success" << endl;
//
//}
////OK
//
//void tests::tsC_case1_input() {
//	TransportCatalogue cat;
//	std::fstream fin;
//	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\pt3\\tsC_case1_input.txt");
//
//	if (fin) {
//		Data(fin, cat);
//	}
//
//	std::fstream fout;
//	fout.open("tsC_output", 'w');
//
//	LOG_DURATION_STREAM("tsC_case1_input", std::cerr);
//	if (fin && fout) {
//		GetRequest(fin, fout, cat);
//	}
//
//	cout << "tsC_case1_input success" << endl;
//}
////OK
//
//void tests::PatrCCase2Input() {
//	TransportCatalogue cat;
//	std::fstream fin;
//	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\part_c_case\\PatrCCase2Input.txt");
//
//	if (fin) {
//		Data(fin, cat);
//	}
//
//	std::fstream fout;
//	fout.open("tsC_output", 'w');
//
//	LOG_DURATION_STREAM("PatrC_output", std::cerr);
//	if (fin && fout) {
//		GetRequest(fin, fout, cat);
//	}
//
//	cout << "PatrCCase2Input success" << endl;
//}
////OK
//
//
//
//using namespace tests;
//
//void speed::AddStop() {
//	{
//		TransportCatalogue cat;
//		double lat = 0.2222222;
//		double lon = 0.4444444;
//
//		LOG_DURATION_STREAM("AddStop", std::cerr);
//		for (int i = 0; i < 500; ++i) {
//			string is = std::to_string(i);
//			cat.AddStop(is, lat, lon);
//			++lat; ++lon;
//		}
//	}
//	cout << "AddStop() success" << endl;
//}
////OK
//
//void speed::AddBus() {
//	using namespace std::literals;
//	{
//		TransportCatalogue cat;
//		double lat = 0.2222222;
//		double lon = 0.4444444;
//		string fir = "1"s;
//		auto m = cat.AddStop(fir, lat, lon);
//
//		vector<Stops*> stops;
//
//		LOG_DURATION_STREAM("AddBus", std::cerr);
//		for (int a = 1; a < 100; ++a) {
//			stops.clear();
//
//			for (int i = a * 2; i <= a * 20; ++i) {
//				string is = std::to_string(i);
//				auto m = cat.AddStop(is, lat++, lon++);
//				stops.push_back(cat.FindStop(std::to_string(i)));
//			}
//			auto bus = cat.AddBus(std::to_string(a), move(stops));
//		}
//	}
//	cout << "AddBus() success" << endl;
//}
////OK
//
//void speed::FindStop() {
//	{
//		TransportCatalogue cat;
//		double lat = 0.2222222;
//		double lon = 0.4444444;
//
//		for (int i = 0; i < 250; ++i) {
//			string is = std::to_string(i);
//			cat.AddStop(is, lat, lon);
//			++lat; ++lon;
//		}
//
//		lat = 0.2222222;
//		lon = 0.4444444;
//
//		LOG_DURATION_STREAM("FindStop", std::cerr);
//		for (int i = 0; i < 25'000; ++i) {
//			cat.FindStop(std::to_string(i));
//		}
//	}
//	cout << "FindStop() success" << endl;
//}
////OK
//
//void speed::FindBus() {
//	{
//		TransportCatalogue cat;
//		double lat = 0.2222222;
//		double lon = 0.4444444;
//		string fir = "1"s;
//		auto m = cat.AddStop(fir, lat, lon);
//
//		vector<Stops*> stops;
//		for (int a = 1; a < 100; ++a) {
//			stops.clear();
//
//			for (int i = a * 2; i <= a * 20; ++i) {
//				string is = std::to_string(i);
//				auto m = cat.AddStop(is, lat++, lon++);
//				stops.push_back(cat.FindStop(std::to_string(i)));
//			}
//			auto bus = cat.AddBus(std::to_string(a), move(stops));
//		}
//
//		LOG_DURATION_STREAM("AddBus", std::cerr);
//		for (int a = 1; a < 100; ++a) {
//			cat.FindBus(std::to_string(a));
//		}
//	}
//	cout << "FindBus() success" << endl;
//}
////OK
//
//void speed::ReadData() {
//	TransportCatalogue cat;
//	std::ifstream fin;
//	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\pt1\\tsA_case2_input.txt");
//
//	if (fin) {
//		LOG_DURATION_STREAM("ReadData", std::cerr);
//		Data(fin, cat);
//	}
//
//	cout << "ReadData() success" << endl;
//}
////OK
//
//void speed::ShowData() {
//	TransportCatalogue cat;
//	std::ifstream fin;
//	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\pt1\\tsA_case2_input.txt");
//
//	if (fin) {
//		Data(fin, cat);
//	}
//
//	std::fstream fout;
//	fout.open("tsA_case2_output", 'w');
//
//	vector<string> all_buses = move(cat.GetAllBuses());
//	LOG_DURATION_STREAM("ShowData", std::cerr);
//	for (auto& bus : all_buses) {
//		Bus(fout, move(bus), cat);
//	}
//
//	cout << "ShowData() success" << endl;
//}
////OK



//OK
void for_json::Test() {
	Test1();
	Test2();
}

//OK
void for_json::Test1() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\test1\\in.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out1", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test1", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test1() success" << endl;
}

//OK
void for_json::Test2() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\test2\\in.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out2", 'w');

		if (fout) {
			LOG_DURATION_STREAM("Test2", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test2() success" << endl;
}



//OK
void for_svg::Test() {
	Test1();
	Test20();
	Test21();
	Test23();
	Test2();
	TestFull23();
}

void for_svg::Test1() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\test_svg1\\in.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_svg1", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_SVG_1", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_SVG_1() success" << endl;
}
//OK

void for_svg::Test20() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\test_svg20\\in.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_svg20", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_SVG_20", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_SVG_20() success" << endl;
}
//OK

void for_svg::Test21() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\test_svg21\\in.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_svg21", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_SVG_21", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_SVG_21() success" << endl;
}
//OK

void for_svg::Test23() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\test_svg23\\in.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_svg23", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_SVG_23", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_SVG_23() success" << endl;
}
//OK

void for_svg::Test2() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\test_svg2\\in.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_svg2", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_SVG_23", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_SVG_2() success" << endl;
}
//OK

void for_svg::TestFull23() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\test_svg_full_23\\in.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_svg_full_23", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_SVG_23", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_SVG_FULL_23() success" << endl;
}
//OK


//
void finall::Test() {
	TestShort();
	Test1();
	Test2();
	Test3();
}

void finall::TestShort() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\test_full_short\\in.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_full_short", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_FULL_SHORT", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_FULL_SHORT() success" << endl;
}

//
void finall::Test1() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\test_full\\in1.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_full_1", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_FULL_1", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_FULL_1() success" << endl;
}
//

void finall::Test2() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\test_full\\in2.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_full_2", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_FULL_2", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_FULL_2() success" << endl;
}
//

void finall::Test3() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\test_full\\in3.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_full_3", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_FULL_3", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_FULL_3() success" << endl;
}
//


void route::TestExample1() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory — github + builder SPRINT 12\\tests\\example1.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_example_router1", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_Example_1", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_Example_1() success" << endl;
}

void route::TestExample2() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory — github + builder SPRINT 12\\tests\\example2.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_example_router2", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_Example_2", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_Example_2() success" << endl;
}

void route::TestExample3() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory — github + builder SPRINT 12\\tests\\example3.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_example_router3", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_Example_3", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_Example_3() success" << endl;
}

void route::TestExample4() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory — github + builder SPRINT 12\\tests\\example4.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_example_router4", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_Example_4", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_Example_4() success" << endl;
}

void route::TestAccidentally1() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory — github + builder SPRINT 12\\s12_final_opentest\\s12_final_opentest_1.json");


	if (fin) {
		std::fstream fout;
		fout.open("out_router_accidently_1", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_Accidentally_1", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_Accidentally_1() success" << endl;
}

void route::TestAccidentally2() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory — github + builder SPRINT 12\\s12_final_opentest\\s12_final_opentest_2.json");


	if (fin) {
		std::fstream fout;
		fout.open("out_router_accidently_2", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_Accidentally_2", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_Accidentally_2() success" << endl;
}

void route::TestAccidentally3() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory — github + builder SPRINT 12\\s12_final_opentest\\s12_final_opentest_3.json");


	if (fin) {
		std::fstream fout;
		fout.open("out_router_accidently_3", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_Accidentally_3", std::cerr);
			JSONReader reader(cat);
			reader.ProcessData(fin, fout);
		}
	}

	cout << "Test_Accidentally_3() success" << endl;
}