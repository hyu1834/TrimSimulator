#include "simulator.h"

Simulator::Simulator(std::vector<Command*> commands)	{
	commandPtr = &commands;
	std::cout<<commandPtr->size()<<"\n";
}

Simulator::~Simulator()	{
	
}

void Simulator::startSimulation()	{
	
}

void Simulator::commandIssuer()	{

}