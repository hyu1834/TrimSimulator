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
#define PROCESS_TIME 2 //in ms


class IO_Command: public Command 	{
	private:
	protected:
	public:
		IO_Command(clock_t issueTime);
		~IO_Command();
		int getCommandType();
		void execute();
};

#endif