#include "simulator.h"

Simulator::Simulator(std::vector<Command*> commands, int maxparallelops = 1)	{
	commandPtr = &commands;
	clock = 0;
	this->maxParallelOps = maxparallelops;
}

Simulator::~Simulator()	{
	
}

void Simulator::startSimulation()	{
	
}