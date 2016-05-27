#include "io_command.h"

IO_Command::IO_Command(double issueTime, char type, long startingBlock, long nextNBlock) : 
						Command(issueTime, type, startingBlock, nextNBlock) {
}

IO_Command::~IO_Command()	{

}