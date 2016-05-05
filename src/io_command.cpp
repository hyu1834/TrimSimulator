#include "io_command.h"

IO_Command::IO_Command(clock_t issueTime)	{
	this->issueTime = issueTime;

}

IO_Command::~IO_Command()	{

}

int IO_Command::getCommandType()	{
	return IO_COMMAND;
}

void IO_Command::execute()	{
	// put the program to sleep by n second to simulate the command
	// execution process
	// first calculate the resume time by adding N ms to the current time
	clock_t resumeTime = clock() + PROCESS_TIME;
	while(resumeTime >= clock());
}