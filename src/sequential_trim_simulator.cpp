#include "sequential_trim_simulator.h"

Sequential_Trim_Simulator::Sequential_Trim_Simulator(std::vector<Command*>& commands):Simulator(commands)	{

}

Sequential_Trim_Simulator::~Sequential_Trim_Simulator()	{
	commandPtr = NULL;
}

void Sequential_Trim_Simulator::startSimulation(double readProcessTime, double writeProcessTime, double trimProcessTime)	{
	int commandCounter = 0;
	bool driverBusy = false;
	double driverBusyTime = 0, totalBusyTime = 0;

	// std::cout<<commandPtr->size()<<"\n";
	// if there are no more command to issue, end the simulation
	while(1)	{
		if(commandCounter < commandPtr->size())	{
			Command* nextCommand = commandPtr->at(commandCounter);
			if(nextCommand->getIssueTime() <= clock)	{
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
		}

		// std::cout<<commandCounter<<"\n";

		// check if simulator can execute any command
		if(!driverBusy)	{
			Trim_Command* nextTrimCommand = NULL;
			IO_Command* nextIOCommand = NULL;

			// check both queue, see which one is not empty
			if(!trimQueue.empty())	{
				nextTrimCommand = trimQueue.front();
			}
			if(!ioQueue.empty())	{
				nextIOCommand = ioQueue.front();
			}

			// if both there are command from either queue
			if(nextTrimCommand != NULL && nextIOCommand != NULL)	{
				double timeDiff = nextIOCommand->getIssueTime() - nextTrimCommand->getIssueTime();
				// TRIM
				if(timeDiff > 0)	{
					// execute the command
					driverBusyTime += trimProcessTime;

					// pop it from the queue
					trimQueue.pop();
				}
				// Normal IO
				else	{
					driverBusyTime += (nextIOCommand->getType() == WRITE_COMMAND)?writeProcessTime:readProcessTime;
					ioQueue.pop();
				}
			}
			// if there is some trim command in trim queue
			else if(nextTrimCommand != NULL)	{
				// execute the command
				driverBusyTime += trimProcessTime;
				// pop it from the queue
				trimQueue.pop();
			}
			// if there is some trim command in io queue
			else if(nextIOCommand != NULL)	{
				driverBusyTime += (nextIOCommand->getType() == WRITE_COMMAND)?writeProcessTime:readProcessTime;

				// driverBusyTime = nextIOCommand->getProcessTime();
				ioQueue.pop();
			}

			// set driver to be busy
			driverBusy = true;
		}
		else	{
			totalBusyTime += CLOCK_SPEED;
			driverBusyTime -= CLOCK_SPEED;
			if(driverBusyTime <= 0)	{
				driverBusy = false;
			}
		}

		// std::cout<<driverBusy<<"  "<<driverBusyTime<<"\n";
		if((commandCounter == commandPtr->size()) && !driverBusy && driverBusyTime <= 0)	{
			break;
		}
		clock += CLOCK_SPEED;
	}

	std::cout<<"System was busy with I/O "<< totalBusyTime/clock*100<<"%% of time\n";

}