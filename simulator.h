// simulator.h
#pragma once
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <list>
#include "process.h"

//IDTime is essentially a pair of string and int (useful for returning 2 values)
struct IDTime {
	std::string id;
	int time;
};

// ============================================================================
// SIM ========================================================================
// ============================================================================

class Simulator {
	std::vector<class Process *> procs;
	std::map<std::string, class Process *> procMap;

	class ReadyQueue *rq;
	int doneCount;
	int preemptCount = 0;
	int contextSCount = 0;
	float waitTime = 0;
	float lastLeft = 0;
	float TT = 0;
	float WT = 0;


public:
	Simulator(const std::vector<class Process *>); //simulator takes in a vector
	void pprint();
	void simulate(ReadyQueue * rq, int time_slice, std::string name); //run sim
	int getPreempt();
	int getContextS();
	float getTT();
	float getWT();
};

// ============================================================================
// IO =========================================================================
// ============================================================================

class IOSim {
	std::vector<struct IDTime> procs;
public:
	int append(std::string id, int time); //return success
	std::vector<std::string> cycle(); //return vector of finished procs
	void pprint();
};

// ============================================================================
// CPU ========================================================================
// ============================================================================

class CPUSim {
	int status; // 0,1,2,3 (ready, busy, loading, unloading)
	int t_slice; //allowed time slice
	int load_time;
	int unload_time;

	std::string current_id;
	int time_elapsed; //increases with each cycle
	int current_burst_time; //decreases with each cycle
	int cswitch_count; //count cycles spent context switching

public:
	CPUSim(int ts, int lt, int ut); //ts = 0 or less for no slicing
	int append(std::string id, int time); // (add a burst to the cpusim) return success
	IDTime * cycle(); //(subtract one from time, or load/unload) return burst (done), or NULL for not done
	int getStatus(); //returns current status
};

// ============================================================================
// MEMORY =====================================================================
// ============================================================================

struct ProcMem {
	int originalBurstTime;
	int remainingBursts;
	int timeRemaining;
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
	std::list<class Process *> procs;
public:
	virtual void append(class Process * proc); //add a proc to queue
	virtual std::string printQueue();
	virtual class Process * getNext(); //will return NULL if no next
	virtual class Process * peek(); //will return NULL if no next
};

class FIFOQueue: public ReadyQueue {
	std::list<class Process *> procs;
public:
	void append(class Process * proc);
	virtual std::string printQueue();
	class Process * getNext(); //will return NULL if no next
	class Process * peek(); //will return NULL if no next
};

class SJFQueue: public ReadyQueue {
	std::list<class Process *> procs;
public:
	void append(class Process * proc);
	virtual std::string printQueue();
	class Process * getNext(); //will return NULL if no next
	class Process * peek(); //will return NULL if no next

};
