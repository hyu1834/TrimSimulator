#include "command.h"

Command::Command()	{

}

Command::Command(float issueTime, char type)	{
	this->issueTime = issueTime;
	this->type = type;
}

Command::~Command()	{

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