#include "trim_command.h"

Trim_Command::Trim_Command(clock_t issueTime)	{
	this->issueTime = issueTime;

}

Trim_Command::~Trim_Command()	{

}

int Trim_Command::getCommandType()	{
	return TRIM_COMMAND;
}

void Trim_Command::execute()	{
	// put the program to sleep by n second to simulate the command
	// execution process
	// first calculate the resume time by adding N ms to the current time
	clock_t resumeTime = clock() + PROCESS_TIME;
	while(resumeTime >= clock());
}
