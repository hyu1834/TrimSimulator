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


class Trim_Command: public Command 	{
	private:
	protected:
	public:
		Trim_Command(float issueTime);
		~Trim_Command();
};

#endif