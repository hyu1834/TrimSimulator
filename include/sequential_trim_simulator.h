#ifndef SEQUENTIAL_TRIM_SIMULATOR_H
	#define SEQUENTIAL_TRIM_SIMULATOR_H
//Standard libraries
#include <stdio.h>
#include <queue>
// #include <vector>

//Third parties libraries


//Local libraries
// #include "command.h"
#include "trim_command.h"
#include "io_command.h"
#include "simulator.h"

//MACRO

class Sequential_Trim_Simulator: public Simulator	{
	private:
		// queue for normal io commands
		std::queue<IO_Command*> ioQueue;
		// queue for trim commands
		std::queue<Trim_Command*> trimQueue;

	protected:
	public:
		Sequential_Trim_Simulator(std::vector<Command*>& commands, int maxParallelOps = 1);
		~Sequential_Trim_Simulator();
		void startSimulation(double readProcessTime, double writeProcessTime, double trimProcessTime);
};
	
#endif