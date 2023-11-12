#include "transport_catalogue.h"
#include "json_reader.h"
#include "map_renderer.h"
#include <iostream>

using namespace std;
using namespace transport_catalogue;
using namespace input;
using namespace picture;

int main() {
	TransportCatalogue cat;
	Data(std::cin, std::cout, cat);
}