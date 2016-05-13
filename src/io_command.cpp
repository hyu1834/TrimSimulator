#include "io_command.h"

IO_Command::IO_Command(float issueTime, char type) : Command(issueTime, type) {

}

IO_Command::~IO_Command()	{

}

int IO_Command::getCommandType()	{
	return IO_COMMAND;
}