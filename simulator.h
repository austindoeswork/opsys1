// simulator.h
#pragma once
#include <vector>

#include "process.h"
//every process is identified by it's ID, not it's object
//do not overwrite the original vector of procs

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
	std::vector<struct Process *> procs;
	//int cswitch load
	//int cswitch unload
public:
	Simulator(const std::vector<struct Process *>); //constructor takes in a vector
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
//public
	//cycle (subtract one) return time remaining, or -1 for not done
	//append (add a burst to the cpusim)
};

class Memory { //holds the processes
	// map [ID] READY,RUNNING,BLOCKED 0,1,2
	// map [ID] atime,iotime,... 0,1,2,3
};