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
using namespace input;
using namespace output;
using namespace tests;

double EPSILON = 0.000001;

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
			transport_catalogue::Data(fin, fout, cat);
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
			transport_catalogue::Data(fin, fout, cat);
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
			transport_catalogue::Data(fin, fout, cat);
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
			transport_catalogue::Data(fin, fout, cat);
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
			transport_catalogue::Data(fin, fout, cat);
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
			transport_catalogue::Data(fin, fout, cat);
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
			transport_catalogue::Data(fin, fout, cat);
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
			transport_catalogue::Data(fin, fout, cat);
		}
	}

	cout << "Test_SVG_FULL_23() success" << endl;
}
//OK


//OK
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
			transport_catalogue::Data(fin, fout, cat);
		}
	}

	cout << "Test_FULL_SHORT() success" << endl;
}
//OK

void finall::Test1() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\test_full\\in1.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_full_1", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_FULL_1", std::cerr);
			transport_catalogue::Data(fin, fout, cat);
		}
	}

	cout << "Test_FULL_1() success" << endl;
}
//OK

void finall::Test2() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\test_full\\in2.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_full_2", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_FULL_2", std::cerr);
			transport_catalogue::Data(fin, fout, cat);
		}
	}

	cout << "Test_FULL_2() success" << endl;
}
//OK

void finall::Test3() {
	TransportCatalogue cat;
	std::ifstream fin;
	fin.open("F:\\_visual_studio\\Practikum132_lesson8_Transport_directory\\test_full\\in3.txt");


	if (fin) {
		std::fstream fout;
		fout.open("out_full_3", 'w');
		if (fout) {
			LOG_DURATION_STREAM("Test_FULL_3", std::cerr);
			transport_catalogue::Data(fin, fout, cat);
		}
	}

	cout << "Test_FULL_3() success" << endl;
}
//OK