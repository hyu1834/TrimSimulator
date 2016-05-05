#ifndef TRIM_COMMAND_H
	#define TRIM_COMMAND_H

//Standard Libraries
#include <iostream>
#include <time.h>

//Third Parties Libraries


//Local Includes
#include "command.h"
#include "macro.h"

//MACRO
#define PROCESS_TIME 2 //in ms


class Trim_Command: public Command 	{
	private:
	protected:
	public:
		Trim_Command(clock_t issueTime);
		~Trim_Command();
		int getCommandType();
		void execute();
};

#endif