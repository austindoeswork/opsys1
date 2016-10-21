// parser.h
// Partners: Austin Wilson, Samuel Johnston, Theodore Rice

#pragma once
#include <string.h> //for c string functions
#include <vector> //for vector stuff

#include "process.h"

void PrintFile(char const * filename);
std::vector<class Process *> Parse(char const * filename);
