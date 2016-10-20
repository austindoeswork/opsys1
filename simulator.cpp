// simulator.cpp
#include "simulator.h"

// ============================================================================
// SIM ========================================================================
// ============================================================================

Simulator::Simulator(const std::vector<class Process *> vp) {
	procs = vp;
	int i = 0;
	doneCount = 0;
	for (; i < vp.size(); i++){ //create map
		procMap[vp[i]->ID()] = vp[i];
	}
}

/*
	simulate
		for not all done
		if a cpu is free
			p = get next in ready //depends on method!!!
				if no ready, break
				remove p from ready
			cpu(p)
		if a cpu finishes a proc p
			if 0 remaining
				io(p)
			else push back into q
		if io finishes a proc
			if proc has more bursts
				ready(p)
			else 
				done(p)
*/

void Simulator::simulate(ReadyQueue * rq) {
	CPUSim csim(0, 4, 4);
	Memory mem(procs);
	IOSim isim;


	int cycle = 0;

	for (; doneCount != procs.size(); cycle++) { //TODO while shit isn't done
		// int time = cycle+1;
		//TODO make procs arrive on time
		//im getting pretty fucking lazy at this point
		int i = 0;
		for (; i < procs.size(); i++) {
			if (cycle == procs[i]->arrival_t()) {
				rq->append(procs[i]);
				printf("time %dms: Process %s arrived %s\n", cycle, procs[i]->ID().c_str(),
					rq->printQueue().c_str());
			}
		}

		// printf("cycle:%d cstatus:%d\n", cycle, csim.getStatus());
		//is cpu free
		if (csim.getStatus() == 0) { 
			auto nxt = rq->getNext();
			if (nxt) {	
				auto id = nxt->ID();
				printf("time %dms: Process %s started using the CPU %s\n",
					cycle, nxt->ID().c_str(), rq->printQueue().c_str());
				csim.append(id, mem.getTimeRemaining(id));
			}
		}
		
		int time = cycle + 1; //because we are cycling, we are now "at" time one ahead
		
		IDTime * doneProc = NULL;
		if (csim.getStatus() != 0 && !(csim.getStatus() == 2 && !rq->peek())) {
			doneProc = csim.cycle();
		}
		//did cpu finish a proc
		if (doneProc) {
			int rem = doneProc->time;
			std::string doneID = doneProc->id;
			if (rem == 0) { //finished cpu burst 
				int remainingBursts = mem.decrementBurst(doneID);
				if (remainingBursts == 0) { //done yo!
					printf("time %dms: Process %s terminated %s\n", time, 
						doneID.c_str(), rq->printQueue().c_str());
					doneCount++;
				} else { //not done yet
					printf("time %dms: Process %s completed a CPU burst; %d to go %s %d\n", time, 
						doneID.c_str(), remainingBursts, rq->printQueue().c_str(), csim.getStatus());
					
					if (procMap[doneID]->io_t() == 0) { //no I/O
						rq->append(procMap[doneID]); //put it back in
					} else {
						isim.append(doneID, procMap[doneID]->io_t());
						printf("time %dms: Process %s blocked on I/O until time %dms %s\n", time, 
							doneID.c_str(), time+procMap[doneID]->io_t(), rq->printQueue().c_str());
					}
				}
			} else { //some cpu still remaining
				rq->append(procMap[doneID]); // put it back in
				mem.setTimeRemaining(doneID, rem); //remember
			}
		}

		//did io finish anything
		std::vector<std::string> ioDoneProcs = isim.cycle();
		int io_i = 0;
		for(; io_i<ioDoneProcs.size(); io_i++){ //iterate thru done io's
			std::string doneID = ioDoneProcs[io_i];
			rq->append(procMap[doneID]);
			printf("time %dms: process %s completed I/O %s %d\n", time, doneID.c_str(),
				rq->printQueue().c_str(), csim.getStatus());
		}
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

int IOSim::append(std::string i, int t) {
	IDTime it;
	it.id = i;
	it.time = t+1; //because we cycle on same iteration
	procs.push_back(it);
	return 0;
}

std::vector<std::string> IOSim::cycle() {
	std::vector<std::string> finished;
	std::vector<struct IDTime> unfinished;
	
	int i = 0;
	for (; i < procs.size(); i++) {
		procs[i].time--;
		if (procs[i].time <= 0) {
			finished.push_back(procs[i].id);
		} else {
			unfinished.push_back(procs[i]);
		}
	}

	procs = unfinished;
	return finished;
}

void IOSim::pprint() {
	int i = 0; 
	for (; i < procs.size(); i++) {
		printf("%s ", procs[i].id.c_str());
	}
	printf("\n");
}

// ============================================================================
// CPU ========================================================================
// ============================================================================

CPUSim::CPUSim(int ts, int lt, int ut) {
	t_slice = ts;
	load_time = lt;
	unload_time = ut;
	status = 2; //loading
	cswitch_count = 0;
}

int CPUSim::append(std::string id, int time) {

	if (status != 0) {
		printf("WARNING -- APPENDING TO BUSY CPU\n");
		return -1;
	}

	status = 1; //busy
	current_burst_time = time;
	current_id = id;
	time_elapsed = 0;

	return 0;
}

int CPUSim::getStatus() {
	return status;
}

IDTime * CPUSim::cycle() {
	//status -- READY
	if (status == 0) {
		printf("WARNING -- EMPTY CPU CYCLE\n");
		return NULL;
	}
	//status -- RUNNING (BUSY)
	if (status == 1) {
		current_burst_time--;
		time_elapsed++;
		if (current_burst_time <= 0 || (t_slice > 0  && time_elapsed >= t_slice)) {
			IDTime *cb = new IDTime();
			cb->id = current_id;
			cb->time = current_burst_time;
			
			status = 3; //start unloading
			cswitch_count = 0;
			return cb;
		} else {
			return NULL;
		}
	}
	//status -- UNLOADING (BUSY)
	if (status == 3) {
		cswitch_count++;
		if (cswitch_count == unload_time) {
			cswitch_count = 0; //start loading
			status = 2;
		}
		return NULL;
	}
	//status -- LOADING (BUSY)
	if (status == 2) {
		cswitch_count++;
		if (cswitch_count == load_time) {
			status = 0; //READY
		}
		return NULL;
	}
	return NULL;
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
		pm->originalBurstTime = vp[i]->burst_t();
		procMap[vp[i]->ID()] = pm;
	}
}

void Memory::pprint() {
	for(auto it = procMap.cbegin(); it != procMap.cend(); ++it) {
	    std::cout << it->first << " " << it->second->timeRemaining << "\n";
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

void Memory::setTimeRemaining(std::string id, int rem) { //set remaining time on current burst
	procMap[id]->timeRemaining = rem;
}

// ============================================================================
// QUEUE ======================================================================
// ============================================================================

void ReadyQueue::append(class Process * proc){
	procs.push_back(proc);
}

Process* ReadyQueue::getNext(){
	if(procs.empty()){return NULL;}
	auto nxt = procs.back();
	procs.pop_back();
	return nxt;
}

Process* ReadyQueue::peek(){
	if(procs.empty()){
		return NULL;
	}
	return procs.back();
}

std::string ReadyQueue::printQueue(){
	std::string queue = "[Q";
	int i = 0;
	for (std::list<Process *>::iterator it = procs.begin(); it != procs.end(); it++) {
    	queue += " " + (*it)->ID();
		i ++;
	}
	if (i == 0) {
		queue += " empty";
	}
	queue += "]";
	return queue;
}

void FIFOQueue::append(class Process * proc){
	procs.push_back(proc);
}

Process* FIFOQueue::getNext(){
	if(procs.empty()){return NULL;}
	auto nxt = procs.front();
	procs.erase(procs.begin());
	return nxt;
}

Process* FIFOQueue::peek(){
	if(procs.empty()){
		return NULL;
	}
	return procs.front();
}

std::string FIFOQueue::printQueue(){
	std::string queue = "[Q";
	int i = 0;
	for (auto it = procs.begin(); it != procs.end(); it++) {
    	queue += " " + (*it)->ID();
		i ++;
	}
	if (i == 0) {
		queue += " empty";
	}
	queue += "]";
	return queue;
}

void SJFQueue::append(class Process * proc){
	if (procs.size() == 0) {
		procs.push_back(proc);
		return;
	}
	for (auto it = procs.begin(); it != procs.end(); it++){
		if (((*it) -> burst_t()) > proc -> burst_t()) {
			procs.insert(it, proc);
			return;
		}

		if (((*it) -> burst_t()) == proc -> burst_t()) {
			if ((*it) -> ID() < proc -> ID()) {
				procs.insert(it, proc);
				return;
			}
			else {
				procs.insert(++it, proc);
				return;
			}
		}
	}
	procs.push_back(proc);

	return;
}

Process * SJFQueue::getNext(){
	if(procs.empty()){return NULL;}
	auto nxt = procs.front();
	procs.pop_front();
	return nxt;
}
Process * SJFQueue::peek(){
	if(procs.empty()){
		return NULL;
	}
	else{
		return procs.front();
	}
}

std::string SJFQueue::printQueue(){
	std::string queue = "[Q";
	int i = 0;
	for(auto it = procs.begin(); it != procs.end(); ++it) {
    	queue += " " + (*it)->ID();
		i++;
	}
	if (i == 0) {
		queue += " empty";
	}
	queue += "]";
	return queue;
}
