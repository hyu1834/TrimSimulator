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
		// queue for queueable commands
		std::queue<Command*> qacQueue;
		// queue for non-queueable commands
		std::queue<Command*> nqacQueue;
		
		//Stat collection
		double totalIOTime;			//IOTime : time used to process IO command
		double totalTrimTime;		//TrimTime : time used to process trim command

		//average queue length computation
		unsigned long IOqueuelength;
		unsigned long Trimqueuelength;
		unsigned long queuelengthCount;			//number of queue length recorded, average queue length is QueueLength(from above)/queuelengthCount

	protected:
	public:
		Semi_Queued_Trim_Simulator(std::vector<Command*>& commands, int maxParallelOps = 1);
		~Semi_Queued_Trim_Simulator();
		void startSimulation(double readProcessTime, double writeProcessTime, double trimProcessTime);
		void StatCollect();
};
	
#endif