#ifndef SEMI_QUEUED_TRIM_SIMULATOR_H
	#define SEMI_QUEUED_TRIM_SIMULATOR_H
//Standard libraries
#include <stdio.h>
#include <iomanip>
#include <queue>

//Third parties libraries


//Local libraries
#include "trim_command.h"
#include "io_command.h"
#include "simulator.h"


class Semi_Queued_Trim_Simulator: public Simulator	{
	private:
		char* driverServingType;
		// queue for write commands
		std::queue<IO_Command*> writeQueue;
		//queue for read commands
		std::queue<IO_Command*> readQueue;
		// queue for non-queueable commands
		std::queue<Trim_Command*> trimQueue;
		
		//Stat collection
		double totalIOTime;			//IOTime : time used to process IO command
		double totalTrimTime;		//TrimTime : time used to process trim command

		//average queue length computation
		unsigned long writeQueueLength;
		unsigned long readQueueLength;
		unsigned long trimQueueLength;
		unsigned long queueLengthCount;			//number of queue length recorded, average queue length is QueueLength(from above)/queuelengthCount

	protected:
	public:
		Semi_Queued_Trim_Simulator(std::vector<Command*>& commands, int maxParallelOps = 1);
		~Semi_Queued_Trim_Simulator();
		void startSimulation(double readProcessTime, double writeProcessTime, double trimProcessTime);
		void StatCollect();
		bool allNonQueueableComplete();
};
	
#endif