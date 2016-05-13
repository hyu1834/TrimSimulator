#ifndef QUEUED_TRIM_SIMULATOR_H
	#define QUEUED_TRIM_SIMULATOR_H
//Standard libraries
#include <queue>

//Third parties libraries


//Local libraries
#include "command.h"
// #include "trim_command.h"
// #include "io_command.h"
#include "simulator.h"


class Queued_Trim_Simulator: public Simulator	{
	private:
		std::queue<Command*> commandQueue;
	protected:
	public:
		Queued_Trim_Simulator(std::vector<Command*>& commands, int parallelProcess = 1);
		~Queued_Trim_Simulator();
		void startSimulation(double readProcessTime, double writeProcessTime, double trimProcessTime);
};
	
#endif