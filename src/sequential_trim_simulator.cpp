#include "sequential_trim_simulator.h"

Sequential_Trim_Simulator::Sequential_Trim_Simulator(std::vector<Command*>& commands, int maxparallelops):Simulator(commands, maxparallelops)	{

}

Sequential_Trim_Simulator::~Sequential_Trim_Simulator()	{
	commandPtr = NULL;
}

void Sequential_Trim_Simulator::startSimulation(double readProcessTime, double writeProcessTime, double trimProcessTime)	{
	int commandCounter = 0;
	char currentServingType = ANY_COMMAND;
	// bool driverBusy = false;
	double totalBusyTime = 0;

	// start simulation
	while(1)	{
		// if there are command that has not been issue yet
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

		// check if simulator can execute any command
		if(!availableDriverSlot.empty())	{
			// while there are available slot, and there are commands in the queue
			while(!availableDriverSlot.empty() && (!trimQueue.empty() || !ioQueue.empty()))	{
				// get the index of driver slot
				int driverSlotIndex = availableDriverSlot.front();
			
				// now check to see which command should be serve first
				Trim_Command* nextTrimCommand = NULL;
				IO_Command* nextIOCommand = NULL;
				// check both queue, see which one is not empty
				if(!trimQueue.empty())	{
					nextTrimCommand = trimQueue.front();
				}
				if(!ioQueue.empty())	{
					nextIOCommand = ioQueue.front();
				}

				// printf("%.9lf\n", clock);
				// std::cout<<"  "<<availableDriverSlot.size()<<"  "<<currentServingType<<"  "<<trimQueue.size()<<"  "<<ioQueue.size()<<"\n";

				double servicesTime = 0.0;
				// if both there are command from either queue
				if(nextTrimCommand != NULL && nextIOCommand != NULL)	{
					double timeDiff = nextIOCommand->getIssueTime() - nextTrimCommand->getIssueTime();
					if(currentServingType == ANY_COMMAND)	{
						// TRIM
						if(timeDiff > 0)	{
							// execute the command
							servicesTime = trimProcessTime;
							// pop it from the queue
							trimQueue.pop();
							currentServingType = TRIM_COMMAND;
						}
						// Normal IO
						else	{
							// set the driver busy time
							servicesTime = (nextIOCommand->getType() == WRITE_COMMAND)?writeProcessTime:readProcessTime;
							// pop the command off ioQueue
							ioQueue.pop();
							currentServingType = IO_COMMAND;
						}
					}
					else if(currentServingType == TRIM_COMMAND)	{
						if(timeDiff > 0)	{
							// execute the command
							servicesTime = trimProcessTime;
							// pop it from the queue
							trimQueue.pop();
						}
					}
					else if(currentServingType == IO_COMMAND)	{
						if(timeDiff < 0)	{
							// set the driver busy time
							servicesTime = (nextIOCommand->getType() == WRITE_COMMAND)?writeProcessTime:readProcessTime;
							// pop the command off ioQueue
							ioQueue.pop();
						}
					}
				}
				// if there is some trim command in trim queue
				else if(nextTrimCommand != NULL)	{
					if(currentServingType == TRIM_COMMAND || currentServingType == ANY_COMMAND)	{
						// execute the command
						servicesTime = trimProcessTime;
						// pop it from the queue
						trimQueue.pop();
					}
				}
				// if there is some trim command in io queue
				else if(nextIOCommand != NULL)	{
					if(currentServingType == IO_COMMAND || currentServingType == ANY_COMMAND)	{
						servicesTime = (nextIOCommand->getType() == WRITE_COMMAND)?writeProcessTime:readProcessTime;
						// pop the command off ioQueue
						ioQueue.pop();
					}
				}
				// set the slot with services time
				if(servicesTime > 0.0)	{
					setDriverBusyTimer(driverSlotIndex, servicesTime);
					// pop it off the queue
					availableDriverSlot.pop();
				}
			}
		}
		else	{
			totalBusyTime += CLOCK_SPEED;
			advanceDriverBusyTime();
			// driverBusyTime -= CLOCK_SPEED;
			if(allCompleted())	{
				currentServingType = ANY_COMMAND;
				printf("all completed at: %.9lf, totalBusyTime: %.9lf\n", clock, totalBusyTime);
			}
		}

		// std::cout<<driverBusy<<"  "<<driverBusyTime<<"\n";
		if((commandCounter == commandPtr->size()) && trimQueue.empty() && ioQueue.empty() && allCompleted())	{
			break;
		}

		advanceClock();
	}

	std::cout<<"System was busy with I/O "<< totalBusyTime/clock*100<<"% of time\n";

}