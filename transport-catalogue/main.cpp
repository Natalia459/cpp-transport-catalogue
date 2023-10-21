//#include "tests.h"

#include "transport_catalogue.h"
#include "input_reader.h"
#include "stat_reader.h"

#include <iostream>

using namespace std;
using namespace transport_catalogue;
using namespace input;
using namespace output;


int main()
{
	//tests::Tests();
	TransportCatalogue cat;
	
	Data(std::cin, cat);
	Request(std::cin, std::cout, cat);

	return 0;
}