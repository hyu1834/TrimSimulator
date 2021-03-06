#include "simulator.h"

Simulator::Simulator(std::vector<Command*>& commands, int maxparallelops)	{
	commandPtr = &commands;
	clock = 0;
	// maximum parallel operation the driver can support
	maxParallelOps = maxparallelops;
	driverBusyTime = new double[maxParallelOps];
	
	// append available driver slot to the queue
	for(int i = 0; i < maxParallelOps; i++)	{
		driverBusyTime[i] = 0.0;
		availableDriverSlot.push(i);
	}

	totalBlockingTime = 0;
	totalBusyTime = 0;
	totalIdleTime = 0;
	currentServingType = ANY_COMMAND;
}

Simulator::~Simulator()	{
	
}

void Simulator::startSimulation(double readProcessTime, double writeProcessTime, double trimProcessTime)	{
	
}

void Simulator::advanceClock()	{
	clock += CLOCK_SPEED;
}

void Simulator::advanceDriverBusyTime()	{
	for(int i = 0; i < maxParallelOps; i++)	{
		//only decrement and check re-add if it's busy
		if(driverBusyTime[i] > 0.0) {
			//advance each driver time by clock speed
			driverBusyTime[i] -= CLOCK_SPEED;
			//if the driver time became <= 0, then put it back to abailableDriverSlot, reset busy time
			if(driverBusyTime[i] <= 0.0 && availableDriverSlot.size() != maxParallelOps)	{
				availableDriverSlot.push(i);
				driverBusyTime[i] = 0.0;
			}
		}
	}
}

void Simulator::advanceOldDriverBusyTime(int newThread)	{
	for(int i = 0; i < maxParallelOps; i++)	{
		if(i == newThread)	continue;
		// advance each driver time by clock speed
		driverBusyTime[i] -= CLOCK_SPEED;
		// if the driver time became <= 0, then put it back to abailableDriverSlot
		if(driverBusyTime[i] <= 0.0)	{
			availableDriverSlot.push(i);
		}
	}

}

void Simulator::setDriverBusyTimer(int operation, double time)	{
	driverBusyTime[operation] = time;
}

// bool Simulator::isAvailableSlot()	{
// 	for(int i = 0; i < maxParallelOps; i++)	{
// 		// if any one of the time is < 0.0 then is there is available slot
// 		if(driverBusyTime[i] <= 0.0)	{
// 			return true;
// 		}
// 	}
// 	return false;
// }

bool Simulator::allCompleted()	{
	return (availableDriverSlot.size() == maxParallelOps);
}

void Simulator::StatCollect()
{

}
