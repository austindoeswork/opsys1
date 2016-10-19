//main.cpp

#include <iostream>

#include "parser.h"
#include "simulator.h"

/*
		Define n as the number of processes to simulate. Note that this is determined via the input
		file described below.

		Define m as the  (i.e., cores) available within the CPU (use a default
		value of 1). Note that we could use this in a future project.

		Define t_cs as the time, in milliseconds, that it takes to perform a context switch (use a
		default value of 8). Remember that a context switch occurs each time a process leaves the
		CPU and is replaced by another process.

		For the RR algorithm, define the aforementioned time slice (i.e., t_slice) value, measured
		in milliseconds, with a default value of 84.
*/

int m = 1; //number of processors (not needed for this project)
int t_slice = 84; //rr slice time (milliseconds)
int t_cs = 8; //context switch time (milliseconds)
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
int main(int argc, char const *argv[])
{
	if (argc < 2) {
		std::cerr << "USAGE: ./a.out <input/file>\n";
		return 1;
	}
	// PrintFile(argv[1]);
	std::vector< class Process* > vp = Parse(argv[1]);

	FIFOQueue rq;
	Simulator sim(vp);
	sim.simulate(&rq);
	// sim.pprint();

	// CPUSim csim(10,5,5);
	// csim.append("A", 168);

	// IDTime * cburst = csim.cycle();
	// for (; !cburst; cburst = csim.cycle()) {
	// 		printf("%s\n", "not done yet.");
	// }
	// if (cburst) {
	// 		printf("%s\n", cburst->id.c_str());
	// 		printf("%d\n", cburst->time);
	// }


	// Memory mem(vp);
	// mem.pprint();
	// mem.decrementBurst("A");
	// printf("%d\n", mem.decrementBurst("A"));

	// IOSim isim;
	// isim.append("A", 1);
	// isim.append("B", 2);
	// isim.append("C", 2);
	// auto v = isim.cycle();
	// printf("%lu\n", v.size());
	// v = isim.cycle();
	// printf("%lu\n", v.size());
	// v = isim.cycle();
	// printf("%lu\n", v.size());

	SJFQueue rq2;
	std::cout << rq2.printQueue();
	rq2.append(vp[2]);
	// std::cout << rq2.printQueue();
	rq2.append(vp[0]);
	rq2.append(vp[1]);
	std::cout << rq2.printQueue();
	printf("\nAverage time is %f\n", aveCpuTime(vp));
}
