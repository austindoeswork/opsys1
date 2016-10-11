// simulator.h
#pragma once
#include <vector>

#include "process.h"
//every process is identified by it's ID, not it's object
//do not overwrite the original vector of procs
//wait on timing? or do we need to think about it in init implementation

/*
	simulate
		for not all done
		if a cpu is free
			p = get next in ready //depends on method!!!
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
	// int clock //count cycles
	std::vector<class Process *> procs; //all procs in here?
	// ReadyQueue rq;
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
//public:
	//cycle (subtract one from each burst) return vector of completed bursts
	//append (add a burst to the iosim)
};

class CPUSim {
	//int num cores?
	//int t_slice
	//int time-left?
//public:
	//cycle (subtract one) return time remaining, or -1 for not done
	//append (add a burst to the cpusim)
};

class ReadyQueue {
protected:
	std::vector<class Process *> procs;
public:
	virtual class Process * getNext(); //will return NULL if no next
};

class FIFOQueue: public ReadyQueue {
public:
	class Process * getNext(); //will return NULL if no next

};

class SJFQueue: public ReadyQueue {
public:
	class Process * getNext(); //will return NULL if no next

};

class RRQueue: public ReadyQueue {
public:
	class Process * getNext(); //will return NULL if no next
};




