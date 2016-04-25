#include "io_utils.h"

void error_message(std::string em)	{
	std::cerr << "Error: " << em << "\n";
	exit(-1);
}

void usage_message(std::string um)	{
	std::cerr << "Usage: " << um << "\n";
	exit(-1);
}