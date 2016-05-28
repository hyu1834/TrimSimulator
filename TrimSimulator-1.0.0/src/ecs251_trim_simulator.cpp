#include "ecs251_trim_simulator.h"

ECS_251_Trim_Simulator::ECS_251_Trim_Simulator(std::vector<Command*>& commands, int parallelProcess) : Simulator(commands, maxParallelOps)
{
	totalIOTime = 0;
	totalTrimTime = 0;
	totalMixTime = 0;
	IOqueuelength = 0;
	Trimqueuelength = 0;
	queuelengthCount = 0;
}
ECS_251_Trim_Simulator::~ECS_251_Trim_Simulator()
{

}

void ECS_251_Trim_Simulator::startSimulation(double readProcessTime, double writeProcessTime, double trimProcessTime)
{
	int commandCounter = 0;

	// bool driverBusy = false;


	// start simulation
	while (1) {
		// if there are command that has not been issue yet
		if (commandCounter < commandPtr->size()) {
			Command* nextCommand = commandPtr->at(commandCounter);
			if (nextCommand->getIssueTime() <= clock) {
				// put the command in to queue
				if (nextCommand->getType() == TRIM_COMMAND) {
					trimQueue.push((Trim_Command*)nextCommand);
				}
				else {
					ioQueue.push((IO_Command*)nextCommand);
				}
				// move on to the next command
				commandCounter++;
			}
		}

		//compute average queue length
		//record every QUEUE_LENGTH_RES
		if (clock - QUEUE_LENGTH_RES * CLOCK_SPEED * queuelengthCount > 0 && clock > CLOCK_SPEED)
		{
			IOqueuelength += ioQueue.size();
			Trimqueuelength += trimQueue.size();
			queuelengthCount++;
		}

		advanceDriverBusyTime();
		// check if simulator can execute any command
		if (!availableDriverSlot.empty()) {
			// while there are available slot, and there are commands in the queue
			while (!availableDriverSlot.empty() && (!trimQueue.empty() || !ioQueue.empty())) {

				// get the index of driver slot
				int driverSlotIndex = availableDriverSlot.front();

				// now check to see which command should be serve first
				Trim_Command* nextTrimCommand = NULL;
				IO_Command* nextIOCommand = NULL;
				// check both queue, see which one is not empty
				if (!trimQueue.empty()) {
					nextTrimCommand = trimQueue.front();
				}
				if (!ioQueue.empty()) {
					nextIOCommand = ioQueue.front();
				}

				// printf("%.9lf\n", clock);
				// std::cout<<"  "<<availableDriverSlot.size()<<"  "<<currentServingType<<"  "<<trimQueue.size()<<"  "<<ioQueue.size()<<"\n";

				double servicesTime = 0.0;
				// if both there are command from either queue
				if (nextTrimCommand != NULL && nextIOCommand != NULL) {
					double timeDiff = nextIOCommand->getIssueTime() - nextTrimCommand->getIssueTime();
					if (currentServingType == ANY_COMMAND) {
						// TRIM
						if (timeDiff > 0) {
							// execute the command
							servicesTime = trimProcessTime;
							// pop it from the queue
							trimQueue.pop();
							currentServingType = TRIM_COMMAND;
						}
						// Normal IO
						else {
							// set the driver busy time
							servicesTime = (nextIOCommand->getType() == WRITE_COMMAND) ? writeProcessTime : readProcessTime;
							// pop the command off ioQueue
							ioQueue.pop();
							currentServingType = IO_COMMAND;
						}
					}
					else if (currentServingType == TRIM_COMMAND || currentServingType == (TRIM_COMMAND | IO_COMMAND)) {
						if (timeDiff > 0) {
							// execute the command
							servicesTime = trimProcessTime;
							// pop it from the queue
							trimQueue.pop();
							currentServingType = TRIM_COMMAND;
						}
						else
						{
							break;
						}
					}
					else if (currentServingType == IO_COMMAND) {
						if (timeDiff < 0) {
							// set the driver busy time
							servicesTime = (nextIOCommand->getType() == WRITE_COMMAND) ? writeProcessTime : readProcessTime;
							// pop the command off ioQueue
							ioQueue.pop();
							currentServingType = IO_COMMAND;
						}
						else
						{
							//----DIFFERENT!!----
							servicesTime = trimProcessTime;
							trimQueue.pop();
							currentServingType = (TRIM_COMMAND | IO_COMMAND);
							//----END CUSTOM ALG---
						}
					}
				}
				// if there is some trim command in trim queue
				else if (nextTrimCommand != NULL) {
					if (currentServingType == TRIM_COMMAND || currentServingType == ANY_COMMAND || currentServingType == (TRIM_COMMAND | IO_COMMAND)) {
						// execute the command
						servicesTime = trimProcessTime;
						// pop it from the queue
						trimQueue.pop();
						currentServingType = TRIM_COMMAND;
					}
					else
					{
						break;
					}
				}
				// if there is some trim command in io queue
				else if (nextIOCommand != NULL) {
					if (currentServingType == IO_COMMAND || currentServingType == ANY_COMMAND) {
						servicesTime = (nextIOCommand->getType() == WRITE_COMMAND) ? writeProcessTime : readProcessTime;
						// pop the command off ioQueue
						ioQueue.pop();
						currentServingType = IO_COMMAND;
					}
					else
					{
						break;
					}
				}
				// set the slot with services time
				if (servicesTime > 0.0) {
					setDriverBusyTimer(driverSlotIndex, servicesTime);
					// pop it off the queue
					availableDriverSlot.pop();
				}
			}
		}
		else {
		}
		if (allCompleted()) {
			currentServingType = ANY_COMMAND;
			//printf("all completed at: %.9lf, totalBusyTime: %.9lf\n", clock, totalBlockingTime);
		}

		// std::cout<<driverBusy<<"  "<<driverBusyTime<<"\n";
		if ((commandCounter == commandPtr->size()) && trimQueue.empty() && ioQueue.empty() && allCompleted()) {
			break;
		}

		StatCollect();
		advanceClock();
	}

	std::cout << "System was blocking " << totalBlockingTime / clock * 100 << "% of time\n";
	std::cout << "System was prcessing IO " << totalIOTime / clock * 100 << "% of time\n";
	std::cout << "System was processing TRIM " << totalTrimTime / clock * 100 << "% of time\n";

	std::cout << "System average IO queue length " << (long double)IOqueuelength / (long double)queuelengthCount << "\n";
	std::cout << "System average Trim queue length " << (long double)Trimqueuelength / (long double)queuelengthCount << "\n";
}

void ECS_251_Trim_Simulator::StatCollect()
{
	//Compute busy/idle time
	if (allCompleted())
	{
		totalIdleTime += CLOCK_SPEED;
	}
	else if (currentServingType == TRIM_COMMAND)
	{
		totalTrimTime += CLOCK_SPEED;
		totalBusyTime += CLOCK_SPEED;
	}
	else if (currentServingType == (TRIM_COMMAND | IO_COMMAND))
	{
		totalMixTime += CLOCK_SPEED;
		totalBusyTime += CLOCK_SPEED;
	}
	else
	{
		totalIOTime += CLOCK_SPEED;
		totalBusyTime += CLOCK_SPEED;
	}

	if (availableDriverSlot.empty() && (!trimQueue.empty() || !ioQueue.empty()))
	{
		totalBlockingTime += CLOCK_SPEED;
	}

	//compute average queue length
	//record every QUEUE_LENGTH_RES
	if (clock - QUEUE_LENGTH_RES * CLOCK_SPEED * queuelengthCount > 0 && clock > CLOCK_SPEED)
	{
		IOqueuelength += ioQueue.size();
		Trimqueuelength += trimQueue.size();
		queuelengthCount++;
	}
}