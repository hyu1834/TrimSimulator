#include "sequential_trim_simulator.h"

Sequential_Trim_Simulator::Sequential_Trim_Simulator(std::vector<Command*> commands):Simulator(commands)	{

}

Sequential_Trim_Simulator::~Sequential_Trim_Simulator()	{

}

void Sequential_Trim_Simulator::startSimulation()	{
	int commandCounter = 0;
	bool lock = false;
	// if there are no more command to issue, end the simulation
	while(commandCounter < commandPtr->size())	{
		if(!lock)	{
			
		}
		else	{
			
		}
		clock += 0.000000001;
	}
}