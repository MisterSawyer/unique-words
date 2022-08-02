#include "utils.h"
#include <iostream>
void printHelp()
{
	std::cout << "\n --- \n";
	std::cout << "Distinct word analyzer\n";
	std::cout << "Usage: [file] [-t] [-x]\n";
	std::cout << "Arguments\n";
	std::cout << "\tfile - path to a file to process\n";
	std::cout << "\t-t=50 - number of threads  \n";
	std::cout << "\t-x - perform test  \n";
}