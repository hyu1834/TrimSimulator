#ifndef HOLD_TRIM_SIMULATOR
#define HOLD_TRIM_SIMULATOR
//Standard libraries
#include <queue>

//Third parties libraries


//Local libraries
#include "command.h"
#include "trim_command.h"
#include "io_command.h"
#include "simulator.h"


class Hold_Trim_Simulator : public Simulator {
private:
	// queue for normal io commands
	std::queue<IO_Command*> ioQueue;
	// queue for trim commands
	std::queue<Trim_Command*> trimQueue;

	//Stat collection
	double totalIOTime;			//IOTime : time used to process IO command
	double totalTrimTime;		//TrimTime : time used to process trim command
								//double totalMixTime;		//MixTime : time when both trim and io command is processed

								//average queue length computation
	unsigned long IOqueuelength;
	unsigned long Trimqueuelength;
	unsigned long queuelengthCount;			//number of queue length recorded, average queue length is QueueLength(from above)/queuelengthCount

protected:
public:
	Hold_Trim_Simulator(std::vector<Command*>& commands, int parallelProcess = 1);
	~Hold_Trim_Simulator();
	void startSimulation(double readProcessTime, double writeProcessTime, double trimProcessTime);
	void StatCollect();
};

#endif
