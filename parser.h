// parser.h

#pragma once
#include <string.h> //for c string functions
#include <vector> //for vector stuff

#include "process.h"

void PrintFile(char const * filename);
std::vector<struct Process *> Parse(char const * filename);
