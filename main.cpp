//main.cpp

#include <iostream>
#include <fstream>

#include "parser.h"
#include "simulator.h"

// int m = 1; //number of processors (not needed for this project)
// int t_slice = 84; //rr slice time (milliseconds)
// int t_cs = 8; //context switch time (milliseconds)

float aveCpuTime(std::vector<class Process*> input){
	float numb = 0;
	float totb = 0;
	float btim = 0;
	float tott = 0;
	for(auto it = input.begin(); it != input.end(); it++){
		numb = (*it) -> num_burst();
		btim = (*it) -> burst_t();
		tott += btim * numb;
		totb += numb;
	}
	return tott/totb;
}

std::string generateStats(std::string queue, float cput) {
	std::string output = "Algorithm " + queue;
	output += "\n -- average CPU burst time: " + std::to_string(cput) + " ms";
	output += "\n -- average wait time: ms";
	output += "\n -- average turnaround time: ms";
	output += "\n -- total number of context switches: ";
	output += "\n -- total number of preemptions: \n";
	return output;
}

int main(int argc, char const *argv[]) {
	if (argc != 3) {
		std::cerr << "USAGE: ./a.out <input-file> <stats-output-file>\n";
		return 1;
	}
	std::vector< class Process* > vp = Parse(argv[1]);

	FIFOQueue rq;
	SJFQueue sq;
	FIFOQueue rrq;

	Simulator sim(vp);
	sim.simulate(&rq, 0, "FCFS");
	printf("\n");

	Simulator sim2(vp);
	sim2.simulate(&sq, 0, "SJF");
	printf("\n");

	Simulator sim3(vp);
	sim3.simulate(&rrq, 84, "RR");

	std::ofstream stats ( argv[2] );

	stats << generateStats("FCFS", aveCpuTime(vp));
	stats << generateStats("SJF", aveCpuTime(vp));
	stats << generateStats("RR", aveCpuTime(vp));

	stats.close();

}
