#ifndef SIMULATOR_H
	#define SIMULATOR_H
//Standard libraries
#include <iostream>
#include <vector>

//Third parties libraries


//Local libraries
#include "command.h"

class Simulator	{
	private:
	protected:
		std::vector<Command*>* commandPtr;
		double clock;
		int maxParallelOps;
	public:
		Simulator(std::vector<Command*> commands, int maxparallelops = 1);
		~Simulator();
		virtual void startSimulation();
};
	
#endif