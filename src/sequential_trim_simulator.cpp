#include "sequential_trim_simulator.h"

Sequential_Trim_Simulator::Sequential_Trim_Simulator(std::vector<Command*> commands):Simulator(commands)	{

}

Sequential_Trim_Simulator::~Sequential_Trim_Simulator()	{

}

void Sequential_Trim_Simulator::startSimulation()	{
	int commandCounter = 0;
	bool driverBusy = false;
	double driverBusyTime = 0, totalBusyTime = 0;


	// if there are no more command to issue, end the simulation
	while(commandCounter < commandPtr->size())	{
		Command* nextCommand = commandPtr->at(commandCounter);
		if(nextCommand->getIssueTime() == clock)	{
			// put the command in to queue
			if(nextCommand->getType() == TRIM_COMMAND)	{
				trimQueue.push((Trim_Command*)nextCommand);
			}
			else	{
				ioQueue.push((IO_Command*)nextCommand);
			}
			// move on to the next command
			commandCounter++;
		}

		// check if simulator can execute any command
		if(!driverBusy)	{
			Trim_Command* nextTrimCommand;
			IO_Command* nextIOCommand;

			// check both queue, see which one is not empty
			if(!trimQueue.empty())	{
				nextTrimCommand = trimQueue.front();
			}
			if(!ioQueue.empty())	{
				nextIOCommand = ioQueue.front();
			}

			// if both there are command from either queue
			if(nextTrimCommand && nextIOCommand)	{
				double timeDiff = nextIOCommand->getIssueTime() - nextTrimCommand->getIssueTime();
				if(timeDiff > 0)	{
					// execute the command
					driverBusyTime += nextTrimCommand->getProcessTime();

					// pop it from the queue
					trimQueue.pop();
				}
				else	{
					driverBusyTime = nextIOCommand->getProcessTime();
					ioQueue.pop();
				}
			}
			// if there is some trim command in trim queue
			else if(nextTrimCommand)	{
				// execute the command
				driverBusyTime += nextTrimCommand->getProcessTime();
				// pop it from the queue
				trimQueue.pop();
			}
			// if there is some trim command in io queue
			else if(nextIOCommand)	{
				driverBusyTime = nextIOCommand->getProcessTime();
				ioQueue.pop();
			}

			// set driver to be busy
			driverBusy = true;
		}
		else	{
			totalBusyTime += CLOCK_SPEED;
			driverBusyTime -= CLOCK_SPEED;
			if(driverBusyTime == 0)	{
				driverBusy = false;
			}
		}
		clock += CLOCK_SPEED;
	}

	// totalBusyTime/clock;

}