#include "command.h"

Command::Command()	{

}

Command::Command(double issueTime, char type, long startingBlock, long nextNBlock)	{
	this->issueTime = issueTime;
	this->type = type;
	this->startingBlock = startingBlock;
	this->nextNBlock = nextNBlock;
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