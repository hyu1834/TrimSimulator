#include "trim_command.h"

Trim_Command::Trim_Command(double issueTime, long startingBlock, long nextNBlock) : 
						   Command(issueTime, 'T', startingBlock, nextNBlock)	{
}

Trim_Command::~Trim_Command()	{

}