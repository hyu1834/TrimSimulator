#include "queued_trim_simulator.h"

Queued_Trim_Simulator::Queued_Trim_Simulator(std::vector<Command*>& commands, int maxParallelOps) : Simulator(commands, maxParallelOps)	{
	
}


Queued_Trim_Simulator::~Queued_Trim_Simulator()	{

}

void Queued_Trim_Simulator::startSimulation(double readProcessTime, double writeProcessTime, double trimProcessTime) {
	int commandCounter = 0;
	// bool driverBusy = false;
	// double* driverBusyTime = new double[parallelProcess]; 
	// double driverBusyTime = 0, 
	unsigned long queuelength = 0;
	unsigned long queuelengthCount = 0;

	// start simulation
	while (1) {
		// if there are command that has not been issue yet
		if (commandCounter < commandPtr->size()) {
			Command* nextCommand = commandPtr->at(commandCounter);
			if (nextCommand->getIssueTime() <= clock) {
				// put the command in to queue
				commandQueue.push(nextCommand);
				// move on to the next command
				commandCounter++;
			}
		}

		//compute average queue length
		//record every QUEUE_LENGTH_RES
		if (clock - QUEUE_LENGTH_RES * CLOCK_SPEED * queuelengthCount > 0 && clock > CLOCK_SPEED)
		{
			queuelength += commandQueue.size();
			queuelengthCount++;
		}

		// printf("clock: %.9lf\n", clock);
		// std::cout<<"  "<<commandCounter<<"  "<<availableDriverSlot.size()<<"  "<<commandQueue.size()<<"\n";
		// check if simulator can execute any command
		while (!availableDriverSlot.empty() && !commandQueue.empty()) {
			// get the index of driver slot
			int driverSlotIndex = availableDriverSlot.front();
			//get the next command off the queue
			Command* nextCommand = commandQueue.front();
			double servicesTime = 0.0;
			if (nextCommand->getType() == TRIM_COMMAND) {
				servicesTime = trimProcessTime;
			}
			else {
				servicesTime = (nextCommand->getType() == WRITE_COMMAND) ? writeProcessTime : readProcessTime;
			}
			// printf("servicesTime: %.9lf\n", servicesTime);
			// set services time
			setDriverBusyTimer(driverSlotIndex, servicesTime + CLOCK_SPEED);
			//pop the command as in served
			commandQueue.pop();
			// pop it off the queue
			availableDriverSlot.pop();
		}

		if (!allCompleted()) {
			totalBlockingTime += CLOCK_SPEED;
		}
		else {
			// printf("all completed at: %.9lf, totalBusyTime: %.9lf\n", clock, totalBusyTime);
		}

		advanceDriverBusyTime();
		// std::cout<<driverBusy<<"  "<<driverBusyTime<<"\n";
		if ((commandCounter == commandPtr->size()) && commandQueue.empty() && allCompleted()) {
			break;
		}

		advanceClock();
	}

	std::cout << "System was blocking " << totalBlockingTime / clock * 100 << "% of time\n";
	std::cout << "System average queue length " << (long double)queuelength / (long double)queuelengthCount << "\n";
}