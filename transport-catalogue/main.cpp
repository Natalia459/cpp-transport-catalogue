#include "transport_catalogue.h"
#include "json_reader.h"
#include "map_renderer.h"

//#include "tests.h"

#include <iostream>

using namespace std;
using namespace transport_catalogue;
using namespace picture;

//using namespace tests::route;

int main() {
	//TestExample1();

	TransportCatalogue cat;
	JSONReader input(cat);
	input.ProcessData(std::cin, std::cout);
}
