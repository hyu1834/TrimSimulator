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
		std::queue<Command*> storageQueue;
	protected:
	public:
		Queued_Trim_Simulator();
		Queued_Trim_Simulator(int maxOp = 1);
		~Queued_Trim_Simulator();
		void startSimulation();
};
	
#endif