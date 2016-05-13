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
	public:

		Simulator(std::vector<Command*> commands);
		~Simulator();
		virtual void startSimulation();
};
	
#endif