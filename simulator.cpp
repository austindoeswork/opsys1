// simulator.cpp
#include "simulator.h"

// ============================================================================
// SIM ========================================================================
// ============================================================================

Simulator::Simulator(const std::vector<class Process *> vp) {
	procs = vp;
	int i = 0;
	for (; i < vp.size(); i++){ //create map
		procMap[vp[i]->ID()] = vp[i]; 
	}
}

void Simulator::pprint() {
	int i = 0;
	for (; i < procs.size(); i++){
		printf("====================\n");
		procs[i]->pprint();
	}
	printf("====================\n");
}

// ============================================================================
// IO =========================================================================
// ============================================================================

// ============================================================================
// CPU ========================================================================
// ============================================================================

CPUSim::CPUSim(int ts, int lt, int ut) {
	t_slice = ts;
	load_time = lt;
	unload_time = ut;
}

int CPUSim::append(std::string id, int time) {
	current_burst_time = time;
	current_id = id;
	time_elapsed = 0;

	return 0;
}

int CPUSim::getStatus() { //TODO
	return 0;
}

IDTime * CPUSim::cycle() {
	//status -- RUNNING
	current_burst_time--;
	time_elapsed++;
	if (current_burst_time == 0 || (t_slice > 0  && time_elapsed >= t_slice)) {
		IDTime *cb = new IDTime();
		cb->id = current_id;
		cb->time = current_burst_time;
		return cb;
	} else {
		return NULL;
	}
}

// ============================================================================
// MEMORY =====================================================================
// ============================================================================

Memory::Memory(const std::vector<class Process *> vp) {
	int i = 0;
	for (; i < vp.size(); i++){
		ProcMem *pm = new ProcMem();
		pm->remainingBursts = vp[i]->num_burst();
		pm->timeRemaining = vp[i]->burst_t();
		procMap[vp[i]->ID()] = pm; 
	}
}

void Memory::pprint() {
	for(auto it = procMap.cbegin(); it != procMap.cend(); ++it) {
	    std::cout << it->first << " " << it->second->remainingBursts << "\n";
	}
}

int Memory::remainingBursts(std::string id) {
	return procMap[id]->remainingBursts;
}

int Memory::decrementBurst(std::string id) {
	if (procMap.count(id) == 0) {
		return -1;
	}
	procMap[id]->remainingBursts--;
	procMap[id]->timeRemaining = procMap[id]->originalBurstTime; //reset the time
	return procMap[id]->remainingBursts;
}

int Memory::getTimeRemaining(std::string id) { //get remaining time on current burst
	return procMap[id]->timeRemaining;
}

// ============================================================================
// QUEUE ======================================================================
// ============================================================================


