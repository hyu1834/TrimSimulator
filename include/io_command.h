#ifndef IO_COMMAND_H
	#define IO_COMMAND_H

//Standard Libraries
#include <iostream>
#include <time.h>

//Third Parties Libraries


//Local Includes
#include "command.h"
#include "macro.h"

//MACRO

class IO_Command: public Command 	{
	private:
	protected:
	public:
		IO_Command(float issueTime, char type);
		~IO_Command();
		// int getCommandType();
};

#endif