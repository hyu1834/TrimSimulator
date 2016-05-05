#include "command.h"

Command::Command()	{

}

Command::Command(clock_t issueTime)	{
	this->issueTime = issueTime;
}

Command::~Command()	{

}

int Command::getCommandType()	{
	return COMMAND;
}

void Command::execute()	{
	
}

clock_t Command::getIssueTime()	{
	return issueTime;
}
