#include "command.h"

Command::Command()	{

}

Command::Command(float issueTime, char type)	{
	this->issueTime = issueTime;
	this->type = type;
}

Command::~Command()	{

}

int Command::getCommandType()	{
	return COMMAND;
}

void Command::execute()	{
	
}

float Command::getIssueTime()	{
	return issueTime;
}

float Command::getProcessTime() {
	return processTime;
}

char Command::getType() {
	return type;
}