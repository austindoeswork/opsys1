// process.cpp
#include "process.h"

Process::Process(std::string i, int at, int bt, int n, int it) {
	id = i;
	arrivalTime = at;
	burstTime = bt;
	ioTime = it;
	numBurst = n;
}

void Process::pprint() {
	printf("ID: %s\nArrivalTime: %d\nBurstTime: %d\nIOTime: %d\nNumBurst: %d\n", id.c_str(), arrivalTime, burstTime, ioTime, numBurst);
}