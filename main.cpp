//main.cpp

#include <iostream>

#include "parser.h"

int main(int argc, char const *argv[])
{
	if (argc < 2) {
		std::cerr << "USAGE: ./a.out <input/file>\n";
		return 1;
	}
	PrintFile(argv[1]);
}

