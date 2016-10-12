// simulator.h
#pragma once
#include <string>
#include <vector>
#include <queue>
#include <map>

#include "process.h"
//every process is identified by it's ID, not it's object
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


class Simulator {
	// int cpuclock //count cycles
	std::vector<class Process *> procs; //all procs in here?
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


class IOSim {
	//vector of io bursts
// public:
	//append (add a burst to the iosim)
	//cycle (subtract one from each burst) return vector of completed bursts
};

struct CPUBurst {
	std::string id;
	int time;
};

class CPUSim { //TODO how to deal with load/unload?
	// int status? 0,1,2,3 (running, free, loading, unloading)?
	// int time_left; need this?
	int t_slice;
	int time_elapsed;
	struct CPUBurst current_burst;
public:
	CPUSim(int ts); //0 or less for no slicing
	void append(struct CPUBurst b); // (add a burst to the cpusim)
	CPUBurst * cycle(); //(subtract one from time, or load/unload) return burst (done), or NULL for not done
};

struct ProcMem {
	int remainingBursts;
	//int wait time, ...
};

class Memory {
	std::map<std::string, struct ProcMem*> procMap;
public:
	Memory(const std::vector<class Process *>); //load vector into map by ID
	int remainingBursts(std::string); //num bursts left
	void decrementBurst(std::string); //decrease num bursts by one
	void pprint();
};

class ReadyQueue {
// protected:
	// std::vector<class Process *> procs;
public:
	virtual void append(class Process * proc); //add a proc to queue
	virtual class Process * getNext(); //will return NULL if no next
	virtual class Process * peek(); //will return NULL if no next
};

class FIFOQueue: public ReadyQueue {
	std::queue<class Process *> fifo_q;
public:
	void append(class Process * proc);
	class Process * getNext(); //will return NULL if no next
	class Process * peek(); //will return NULL if no next
};

class SJFQueue: public ReadyQueue {
public:
	class Process * getNext(); //will return NULL if no next
	class Process * peek(); //will return NULL if no next

};

class RRQueue: public ReadyQueue {
public:
	class Process * getNext(); //will return NULL if no next
	class Process * peek(); //will return NULL if no next
};




