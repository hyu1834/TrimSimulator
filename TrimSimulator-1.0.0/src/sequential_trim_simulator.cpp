#include "sequential_trim_simulator.h"

Sequential_Trim_Simulator::Sequential_Trim_Simulator(std::vector<Command*>& commands, int maxparallelops):Simulator(commands, maxparallelops)	{
	totalIOTime = 0;
	totalTrimTime = 0;
	IOqueuelength = 0;
	Trimqueuelength = 0;
	queuelengthCount = 0;
}

Sequential_Trim_Simulator::~Sequential_Trim_Simulator()	{
	commandPtr = NULL;
}

void Sequential_Trim_Simulator::startSimulation(double readProcessTime, double writeProcessTime, double trimProcessTime)	{
	FILE* log;
	log = fopen("../../../sequential_simulator.csv", "w");
	int count = 0;
	int issuedCommandCount = 0;

	// start simulation
	while(1)	{
		// if there are command that has not been issue yet
		if(issuedCommandCount < commandPtr->size())	{
			Command* nextCommand = commandPtr->at(issuedCommandCount);
			if(nextCommand->getIssueTime() <= clock)	{
				// put the command in to queue
				if(nextCommand->getType() == TRIM_COMMAND)	{
					trimQueue.push((Trim_Command*)nextCommand);
				}
				else	{
					ioQueue.push((IO_Command*)nextCommand);
				}
				// move on to the next command
				issuedCommandCount++;
			}
		}
		
		// advance the clock only if there are something running
		if(availableDriverSlot.size() != maxparallelops)	{
			advanceDriverBusyTime();
		}
		
		// check if simulator can execute any command
		while(!availableDriverSlot.empty())	{
			// if theres no command in any queue
			if(trimQueue.empty() && ioQueue.empty())	{
				break;
			}
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

			double servicesTime = 0.0;
			// if both there are command from either queue
			if(nextTrimCommand != NULL && nextIOCommand != NULL)	{
				double timeDiff = nextIOCommand->getIssueTime() - nextTrimCommand->getIssueTime();
				if((timeDiff > 0) && ((currentServingType == ANY_COMMAND) || (currentServingType == TRIM_COMMAND)))	{
					servicesTime = trimProcessTime;
					// pop it from the queue
					trimQueue.pop();
					currentServingType = TRIM_COMMAND;
				}
				else if ((timeDiff < 0) && ((currentServingType == ANY_COMMAND) || (currentServingType == IO_COMMAND)))	{
					// set the driver busy time
					servicesTime = (nextIOCommand->getType() == WRITE_COMMAND)?writeProcessTime:readProcessTime;
					// pop the command off ioQueue
					ioQueue.pop();
					currentServingType = IO_COMMAND;
				}
				else	{
					break;
				}
			}
			else if(nextTrimCommand != NULL)	{
				if((currentServingType == ANY_COMMAND) || (currentServingType == TRIM_COMMAND))	{
					servicesTime = trimProcessTime;
					// pop it from the queue
					trimQueue.pop();
					currentServingType = TRIM_COMMAND;
				}
				else	{
					break;
				}
			}
			else if(nextIOCommand != NULL)	{
				if((currentServingType == ANY_COMMAND) || (currentServingType == IO_COMMAND))	{
					// set the driver busy time
					servicesTime = (nextIOCommand->getType() == WRITE_COMMAND)?writeProcessTime:readProcessTime;
					// pop the command off ioQueue
					ioQueue.pop();
					currentServingType = IO_COMMAND;
				}
				else	{
					break;
				}
			}
			// set the slot with services time
			if(servicesTime > 0.0)	{
				setDriverBusyTimer(driverSlotIndex, servicesTime);
				// pop it off the queue
				availableDriverSlot.pop();
			}
		}

		if (allCompleted()) {
			currentServingType = ANY_COMMAND;
		}

		if(count %10000 == 0)	{
			fprintf(log, "%.10lf,%lu,%lu,%lu,%lu,%d,%lu\n",clock, ioQueue.size(), trimQueue.size(), (unsigned long)maxParallelOps-availableDriverSlot.size(), commandPtr->size(), issuedCommandCount, commandPtr->size()-issuedCommandCount);
			//simLog<<std::setprecision(10)<<clock<<","<<IOqueuelength<<","<<Trimqueuelength<<","<<maxParallelOps-availableDriverSlot.size()<<"\n";
			// std::cout<<commandCounter<<"  "<<commandPtr->size()<<"  "<<trimQueue.empty()<<"  "<<ioQueue.empty()<<"  "<<allCompleted()<<"\n";
		}
		if((issuedCommandCount == commandPtr->size()) && trimQueue.empty() && ioQueue.empty() && allCompleted())	{
			break;
		}
		
		//Stat collection before advancing clock
		StatCollect();
		advanceClock();
		count++;
	}
	fprintf(log, "%.10lf,%lu,%lu,%lu,%lu,%d,%lu\n",clock, ioQueue.size(), trimQueue.size(), (unsigned long)maxParallelOps-availableDriverSlot.size(), commandPtr->size(), issuedCommandCount, commandPtr->size()-issuedCommandCount);

	fclose(log);

	std::cout << std::setprecision(10) << "System was blocking "<< (double)totalBlockingTime / (double)clock * 100.0<<"% of time\n";
	std::cout << std::setprecision(10) << "System was busy "<< (double)totalBusyTime / (double)clock * 100.0<<"% of time\n";
	std::cout << std::setprecision(10) << "System was idle " << (double)totalIdleTime / (double)clock * 100.0 << "% of time\n\n";

	std::cout << std::setprecision(10) << "System was prcessing IO " << (double)totalIOTime / (double)clock * 100.0 << "% of time\n";
	std::cout << std::setprecision(10) << "System was processing TRIM " << (double)totalTrimTime / (double)clock * 100.0 << "% of time\n\n";

	std::cout << std::setprecision(10) << "System average IO queue length " << (long double)IOqueuelength / (long double)queuelengthCount << "\n";
	std::cout << std::setprecision(10) << "System average Trim queue length " << (long double)Trimqueuelength / (long double)queuelengthCount << "\n";
}

void Sequential_Trim_Simulator::StatCollect()
{
	//Compute busy/idle time
	if (allCompleted())
	{
		totalIdleTime += CLOCK_SPEED;
	}
	else	{
		totalBusyTime += CLOCK_SPEED;
		if(currentServingType == TRIM_COMMAND)	{
			totalTrimTime += CLOCK_SPEED;
		}
		else	{
			totalIOTime += CLOCK_SPEED;
		}
	}

	//if either one queue still have command and the available slot > 0
	if (availableDriverSlot.empty() && (!trimQueue.empty() || !ioQueue.empty()))	{
		totalBlockingTime += CLOCK_SPEED;
	}

	//compute average queue length
	//record every QUEUE_LENGTH_RES
	if (((clock - QUEUE_LENGTH_RES * CLOCK_SPEED * queuelengthCount) > 0) && (clock > CLOCK_SPEED))	{
		IOqueuelength += ioQueue.size();
		Trimqueuelength += trimQueue.size();
		queuelengthCount++;
	}
}
