// process.h
#pragma once
#include <string>
#include <iostream>

class Process {
	std::string id;
	int arrivalTime;
	int burstTime;
	int ioTime;
	int numBurst;
public:
	Process(std::string i, int at, int bt, int it, int n);
	void pprint();
};
