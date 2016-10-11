// simulator.cpp
#include "simulator.h"

Simulator::Simulator(const std::vector<struct Process *> vp) {
	procs = vp;
}

void Simulator::pprint() {
	int i = 0;
	for (; i < procs.size(); i++){
		printf("====================\n");
		procs[i]->pprint();
	}
	printf("====================\n");
}
