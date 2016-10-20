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
	int prevTime = 0;
	int waitTime = 0;

public:
	Process(std::string i, int at, int bt, int n, int it);
	const std::string ID() {return id;}
	const int arrival_t() {return arrivalTime;}
	const int burst_t() {return burstTime;}
	const int io_t() {return ioTime;}
	const int num_burst() {return numBurst;}
	int lastleft() {return prevTime;}
	int getWait() {return waitTime;}
	void incWT(int cycle){waitTime += (cycle - prevTime);}
	void setlast(int time){prevTime = time;}
	void pprint();
};
