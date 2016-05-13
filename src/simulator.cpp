#include "simulator.h"

Simulator::Simulator(std::vector<Command*> commands)	{
	commandPtr = &commands;
	clock = 0;
}

Simulator::~Simulator()	{
	
}

void Simulator::startSimulation()	{
	
}