#ifndef SEQUENTIAL_TRIM_SIMULATOR_H
	#define SEQUENTIAL_TRIM_SIMULATOR_H
//Standard libraries
#include <queue>

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
		Sequential_Trim_Simulator();
		Sequential_Trim_Simulator(int maxOp);
		~Sequential_Trim_Simulator();
		void startSimulation();
};
	
#endif