//main.cpp
// Partners: Austin Wilson, Samuel Johnston, Theodore Rice

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "parser.h"
#include "simulator.h"

// int m = 1; //number of processors (not needed for this project)
// int t_slice = 84; //rr slice time (milliseconds)
// int t_cs = 8; //context switch time (milliseconds)

int processIOT(Process * proc) {
	return ((proc->io_t()) * (proc->burst_t()-1));
}

std::vector<float> aveCpuTime(std::vector<class Process*> input){
	float numb = 0;
	float totb = 0;
	float btim = 0;
	float tott = 0;
	std::vector<float> ret;
	for(auto it = input.begin(); it != input.end(); it++){
		numb = (*it) -> num_burst();
		btim = (*it) -> burst_t();
		tott += btim * numb;
		totb += numb;
	}
	ret.push_back(tott);
	ret.push_back(totb);
	ret.push_back(tott/totb);
	return ret;
}

std::string generateStats(std::string queue, float cput, float pc, float wt, float tt, float csc) {
	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << cput;
	std::string ans = ss.str();
	std::string output = "Algorithm " + queue;
	output += "\n -- average CPU burst time: " + ans + " ms";
	output += "\n -- average wait time: " + std::to_string(wt) + " ms" ;
	output += "\n -- average turnaround time: " + std::to_string(tt) + " ms";
	output += "\n -- total number of context switches: " + std::to_string((int) csc);
	output += "\n -- total number of preemptions: " + std::to_string((int) pc) + "\n";
	return output;
}

int main(int argc, char const *argv[]) {
	if (argc != 3) {
		std::cerr << "USAGE: ./a.out <input-file> <stats-output-file>\n";
		return 1;
	}
	std::vector< class Process* > vp = Parse(argv[1]);
	std::vector< class Process* > vp2 = Parse(argv[1]);
	std::vector< class Process* > vp3 = Parse(argv[1]);
	FIFOQueue rq;
	SJFQueue sq;
	FIFOQueue rrq;

	Simulator sim(vp);
	sim.simulate(&rq, 0, "FCFS");
	int pre1 = sim.getPreempt();
	printf("\n");

	Simulator sim2(vp2);
	sim2.simulate(&sq, 0, "SJF");
	int pre2 = sim2.getPreempt();
	printf("\n");

	Simulator sim3(vp3);
	sim3.simulate(&rrq, 84, "RR");
	int pre3 = sim3.getPreempt();

	std::ofstream stats ( argv[2] );
	int csc1 = sim.getContextS();
	int csc2 = sim2.getContextS();
	int csc3 = sim3.getContextS();
	auto vect = aveCpuTime(vp);
	double tt1 = sim.getTT() / sim.getSwaps(); // vect[1];
	double tt2 = sim2.getTT() / sim2.getSwaps(); // vect[1];
	double tt3 = sim3.getTT() / sim3.getSwaps(); // vect[1];
	float wt1 = sim.getWT() / vect[1];
	float wt2 = sim2.getWT() / vect[1];
	float wt3 = sim3.getWT() / vect[1];
	if((vp.size() == 1)){
		wt1 = 0;
		wt2 = 0;
		wt3 = 0;
	}

	stats << generateStats("FCFS", vect[2], pre1, wt1, tt1, csc1);
	stats << generateStats("SJF", vect[2], pre2, wt2, tt2, csc2);
	stats << generateStats("RR", vect[2], pre3, wt3, tt3, csc3);

	stats.close();

}
