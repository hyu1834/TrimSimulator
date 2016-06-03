#include "semi_queued_trim_simulator.h"

Semi_Queued_Trim_Simulator::Semi_Queued_Trim_Simulator(std::vector<Command*>& commands, int maxparallelops):Simulator(commands, maxparallelops)	{
	totalIOTime = 0;
	totalTrimTime = 0;
	IOqueuelength = 0;
	Trimqueuelength = 0;
	queuelengthCount = 0;
}

Semi_Queued_Trim_Simulator::~Semi_Queued_Trim_Simulator()	{
	commandPtr = NULL;
}

void Semi_Queued_Trim_Simulator::startSimulation(double readProcessTime, double writeProcessTime, double trimProcessTime)	{
	FILE* sqt_log;
	sqt_log = fopen("../../../semi_queue_simulator.csv", "w");
	int count = 0;
	int commandCounter = 0;

	// start simulation
	while(1)	{
		// if there are command that has not been issue yet
		if(commandCounter < commandPtr->size())	{
			Command* nextCommand = commandPtr->at(commandCounter);
			if(nextCommand->getIssueTime() <= clock)	{
				// put the command in to queue
				if(nextCommand->getType() == TRIM_COMMAND || nextCommand->getType() == READ_COMMAND)	{
					qacQueue.push(nextCommand);
				}
				else	{
					nqacQueue.push(nextCommand);
				}
				// move on to the next command
				commandCounter++;
			}
		}

		advanceDriverBusyTime();
		// check if simulator can execute any command
		if(!availableDriverSlot.empty())	{
			// while there are available slot, and there are commands in the queue
			while(!availableDriverSlot.empty() && (!qacQueue.empty() || !nqacQueue.empty()))	{

				// get the index of driver slot
				int driverSlotIndex = availableDriverSlot.front();
			
				// now check to see which command should be serve first
				Command* nextQueueableCommand = NULL;
				Command* nextNonQueueableCommand = NULL;
				// check both queue, see which one is not empty
				if(!qacQueue.empty())	{
					nextQueueableCommand = qacQueue.front();
				}
				if(!nqacQueue.empty())	{
					nextNonQueueableCommand = nqacQueue.front();
				}

				double servicesTime = 0.0;
				// if both there are command from either queue
				if(nextQueueableCommand != NULL && nextNonQueueableCommand != NULL)	{
					double timeDiff = nextNonQueueableCommand->getIssueTime() - nextQueueableCommand->getIssueTime();
					if(currentServingType == ANY_COMMAND)	{
						// TRIM
						if(timeDiff > 0)	{
							// execute the command
							servicesTime = (nextQueueableCommand->getType() == TRIM_COMMAND)?trimProcessTime:readProcessTime;
							// pop it from the queue
							qacQueue.pop();
							currentServingType = QUEUEABLE_COMMAND;
						}
						// Normal IO
						else	{
							// set the driver busy time
							servicesTime = writeProcessTime;
							// pop the command off nqacQueue
							nqacQueue.pop();
							currentServingType = NONQUEUEABLE_COMMEND;
						}
					}
					else if(currentServingType == QUEUEABLE_COMMAND)	{
						if(timeDiff > 0)	{
							// execute the command
							servicesTime = (nextQueueableCommand->getType() == TRIM_COMMAND)?trimProcessTime:readProcessTime;
							// pop it from the queue
							qacQueue.pop();
							//currentServingType = QUEUEABLE_COMMAND;
						}
						else
						{
							break;
						}
					}
					else if(currentServingType == NONQUEUEABLE_COMMEND)	{
						if(timeDiff < 0)	{
							// set the driver busy time
							servicesTime = writeProcessTime;
							// pop the command off nqacQueue
							nqacQueue.pop();
							//currentServingType = NONQUEUEABLE_COMMEND;
						}
						else
						{
							break;
						}
					}
				}
				// if there is some trim command in trim queue
				else if(nextQueueableCommand != NULL)	{
					if(currentServingType == QUEUEABLE_COMMAND || currentServingType == ANY_COMMAND)	{
						// execute the command
						servicesTime = (nextQueueableCommand->getType() == TRIM_COMMAND)?trimProcessTime:readProcessTime;
						// pop it from the queue
						qacQueue.pop();
						currentServingType = QUEUEABLE_COMMAND;
					}
					else
					{
						break;
					}
				}
				// if there is some trim command in io queue
				else if(nextNonQueueableCommand != NULL)	{
					if(currentServingType == NONQUEUEABLE_COMMEND || currentServingType == ANY_COMMAND)	{
						servicesTime = writeProcessTime;
						// pop the command off nqacQueue
						nqacQueue.pop();
						currentServingType = NONQUEUEABLE_COMMEND;
					}
					else
					{
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
		}
		else	{
		}
		if (allCompleted()) {
			currentServingType = ANY_COMMAND;
		}

		if(count %10000 == 0)	{
			fprintf(sqt_log, "%.10lf,%lu,%lu,%lu,%lu,%d,%lu\n",clock, nqacQueue.size(), qacQueue.size(), (unsigned long)maxParallelOps-availableDriverSlot.size(), commandPtr->size(), commandCounter, commandPtr->size()-commandCounter);
		}
		if((commandCounter == commandPtr->size()) && qacQueue.empty() && nqacQueue.empty() && allCompleted())	{
			break;
		}
		
		//Stat collection before advancing clock
		StatCollect();
		advanceClock();
		count++;
	}
	fprintf(sqt_log, "%.10lf,%lu,%lu,%lu,%lu,%d,%lu\n",clock, nqacQueue.size(), qacQueue.size(), (unsigned long)maxParallelOps-availableDriverSlot.size(), commandPtr->size(), commandCounter, commandPtr->size()-commandCounter);

	fclose(sqt_log);

	std::cout << std::setprecision(10) << "System was blocking "<< (double)totalBlockingTime / (double)clock * 100.0<<"% of time\n";
	std::cout << std::setprecision(10) << "System was busy "<< (double)totalBusyTime / (double)clock * 100.0<<"% of time\n";
	std::cout << std::setprecision(10) << "System was idle " << (double)totalIdleTime / (double)clock * 100.0 << "% of time\n\n";

	std::cout << std::setprecision(10) << "System was prcessing IO " << (double)totalIOTime / (double)clock * 100.0 << "% of time\n";
	std::cout << std::setprecision(10) << "System was processing TRIM " << (double)totalTrimTime / (double)clock * 100.0 << "% of time\n\n";

	std::cout << std::setprecision(10) << "System average IO queue length " << (long double)IOqueuelength / (long double)queuelengthCount << "\n";
	std::cout << std::setprecision(10) << "System average Trim queue length " << (long double)Trimqueuelength / (long double)queuelengthCount << "\n";
}

void Semi_Queued_Trim_Simulator::StatCollect()
{
	//Compute busy/idle time
	if (allCompleted())
	{
		totalIdleTime += CLOCK_SPEED;
	}
	else if(currentServingType == TRIM_COMMAND)
	{
		totalTrimTime += CLOCK_SPEED;
		totalBusyTime += CLOCK_SPEED;
	}
	else
	{
		totalIOTime += CLOCK_SPEED;
		totalBusyTime += CLOCK_SPEED;
	}

	if (availableDriverSlot.empty() && (!qacQueue.empty() || !nqacQueue.empty()))
	{
		totalBlockingTime += CLOCK_SPEED;
	}

	//compute average queue length
	//record every QUEUE_LENGTH_RES
	if (clock - QUEUE_LENGTH_RES * CLOCK_SPEED * queuelengthCount > 0 && clock > CLOCK_SPEED)
	{
		IOqueuelength += nqacQueue.size();
		Trimqueuelength += qacQueue.size();
		queuelengthCount++;
	}
}
