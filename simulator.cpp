// simulator.cpp
#include "simulator.h"

Simulator::Simulator(const std::vector<class Process *> vp) {
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

CPUSim::CPUSim(int ts) {
	t_slice = ts;
}

void CPUSim::append(struct CPUBurst b) {
	// time_left = b.time;
	current_burst = b;
	time_elapsed = 0;
}

CPUBurst * CPUSim::cycle() {
	current_burst.time--;
	time_elapsed++;
	if (current_burst.time == 0) return &current_burst;
	else if (t_slice > 0  && time_elapsed >= t_slice) {
		return &current_burst;
	} else {
		return NULL;
	}
}

Memory::Memory(const std::vector<class Process *> vp) {
	int i = 0;
	for (; i < vp.size(); i++){
		ProcMem *pm = new ProcMem();
		pm->remainingBursts = vp[i]->num_burst();
		procMap[vp[i]->ID()] = pm; 
	}
}

void Memory::pprint() {
	for(auto it = procMap.cbegin(); it != procMap.cend(); ++it) {
	    std::cout << it->first << " " << it->second->remainingBursts << "\n";
	}
}

int Memory::remainingBursts(std::string id) {
	if (procMap.count(id) == 0) {
		return -1;
	}
	return procMap[id]->remainingBursts;
}

void Memory::decrementBurst(std::string id) {
	if (procMap.count(id) == 0) {
		return;
	}
	procMap[id]->remainingBursts--;
	return;
}





