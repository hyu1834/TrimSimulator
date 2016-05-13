#ifndef SIMULATOR_H
	#define SIMULATOR_H
//Standard libraries
#include <iostream>
#include <vector>
#include <queue>

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
};
	
#endif