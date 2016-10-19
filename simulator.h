// simulator.h
#pragma once
#include <string>
#include <vector>
#include <queue>
#include <map>

#include "process.h"
//every process is identified by it's ID, not its object
//do not overwrite the original vector of procs
//wait on timing? or do we need to think about it in init implementation

/*
	simulate
		for not all done
		if a cpu is free
			p = get next in ready //depends on method!!!
				if no ready, break
				remove p from ready
			cpu(p)
		if a cpu finishes a proc p
			io(p)
		if io finishes a proc
			if proc has more bursts
				ready(p)
			else 
				done(p)
 */

struct IDTime {
	std::string id;
	int time;
};

// ============================================================================
// SIM ========================================================================
// ============================================================================

class Simulator {
	// int cpuclock //count cycles
	std::vector<class Process *> procs; //all procs in here?
	std::map<std::string, class Process *> procMap;



	class ReadyQueue *rq;
	// IOSim io_s;
	// CPUSim cpu_s;

	//int cswitch load
	//int cswitch unload
public:
	Simulator(const std::vector<class Process *>); //conclassor takes in a vector
	void pprint();
	//simulate(context_switch_time, method, ?) //execute simulation
	//reset (reset clock and other stuff)
};

// ============================================================================
// IO =========================================================================
// ============================================================================

class IOSim {
	std::vector<struct IDTime> procs;
public:
	int append(std::string id, int time); //return success
	std::vector<std::string> cycle(); //return vector of finished procs
};

// ============================================================================
// CPU ========================================================================
// ============================================================================


class CPUSim { //TODO how to deal with load/unload?
	// int status // 0,1,2,3 (running, free, loading, unloading)?
	int t_slice; //allowed time slice
	int load_time; //TODO
	int unload_time; //TODO

	int time_elapsed; //increases with each cycle
	std::string current_id;
	int current_burst_time; //decreases with each cycle
public:
	CPUSim(int ts, int lt, int ut); //ts = 0 or less for no slicing
	int append(std::string id, int time); // (add a burst to the cpusim) return success
	IDTime * cycle(); //(subtract one from time, or load/unload) return burst (done), or NULL for not done
	int getStatus();
};

// ============================================================================
// MEMORY =====================================================================
// ============================================================================

struct ProcMem {
	int originalBurstTime;
	int remainingBursts;
	int timeRemaining;

	//int wait time, ...
};

class Memory {
	std::map<std::string, struct ProcMem*> procMap;
public:
	Memory(const std::vector<class Process *>); //load vector into map by ID
	int remainingBursts(std::string); //num bursts left
	int decrementBurst(std::string); //decrease num bursts by one
	int getTimeRemaining(std::string); //get remaining time on current burst
	void setTimeRemaining(std::string, int); //set remaining time on current burst
	void pprint();
};

// ============================================================================
// QUEUE ======================================================================
// ============================================================================

class ReadyQueue {
	std::vector<class Process *> procs;
public:
	virtual void append(class Process * proc); //add a proc to queue
	virtual class Process * getNext(); //will return NULL if no next
	virtual class Process * peek(); //will return NULL if no next
};

class FIFOQueue: public ReadyQueue {
	std::vector<class Process *> procs;
public:
	void append(class Process * proc);
	class Process * getNext(); //will return NULL if no next
	class Process * peek(); //will return NULL if no next
};

class SJFQueue: public ReadyQueue {
	std::vector<class Process *> procs;
public:
	void append(class Process * proc);
	class Process * getNext(); //will return NULL if no next
	class Process * peek(); //will return NULL if no next

};


