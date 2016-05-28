#ifndef SIMULATOR_H
	#define SIMULATOR_H
//Standard libraries
#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>

//Third parties libraries


//Local libraries
#include "macro.h"
#include "command.h"

class Simulator	{
	private:
	protected:
		std::vector<Command*>* commandPtr;
		double clock;
		int maxParallelOps;
		double* driverBusyTime;

		// Stat collection, could have more in derived class
		double totalBlockingTime;			//Blocking : No available driver slot to process command and there's command in queue
		double totalBusyTime;				//Busy : At least one command is being processed
		double totalIdleTime;				//IdleTime : no command is being processed

		char currentServingType;

		std::queue<int> availableDriverSlot;
	public:
		Simulator(std::vector<Command*>& commands, int maxparallelops = 1);
		~Simulator();
		virtual void startSimulation(double readProcessTime, double writeProcessTime, double trimProcessTime);
		void advanceClock();
		void advanceDriverBusyTime();
		void advanceOldDriverBusyTime(int newThread);
		void setDriverBusyTimer(int operation, double time);
		// bool isAvailableSlot();
		bool allCompleted();
		//Stat collection function, to be implemented in derived classes
		virtual void StatCollect();
};
	
#endif