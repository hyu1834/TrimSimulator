#ifndef ECS_251_TRIM_SIMULATOR_H
	#define ECS_251_TRIM_SIMULATOR_H
//Standard libraries
#include <queue>

//Third parties libraries


//Local libraries
#include "command.h"
#include "trim_command.h"
#include "io_command.h"
#include "simulator.h"


class ECS_251_Trim_Simulator : public Simulator {
private:
	// queue for normal io commands
	std::queue<IO_Command*> ioQueue;
	// queue for trim commands
	std::queue<Trim_Command*> trimQueue;
protected:
public:
	ECS_251_Trim_Simulator(std::vector<Command*>& commands, int parallelProcess = 1);
	~ECS_251_Trim_Simulator();
	void startSimulation(double readProcessTime, double writeProcessTime, double trimProcessTime);
};

#endif