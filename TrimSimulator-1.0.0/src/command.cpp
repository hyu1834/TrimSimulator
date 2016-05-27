#include "command.h"

Command::Command()	{

}

Command::Command(double issueTime, char type)	{
	this->issueTime = issueTime;
	this->type = type;
}

Command::~Command()	{

}

double Command::getIssueTime()	{
	return issueTime;
}

// double Command::getProcessTime() {
// 	return processTime;
// }

char Command::getType() {
	return type;
}