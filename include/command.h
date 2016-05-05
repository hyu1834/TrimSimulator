#ifndef COMMAND_H
	#define COMMAND_H

//Standard Libraries
#include <iostream>
#include <time.h>

//Third Parties Libraries


//Local Includes
#include "macro.h"

//MACRO

class Command 	{
	private:
		
	protected:
		clock_t issueTime;
	public:
		Command();
		Command(clock_t issueTime);
		~Command();
		virtual int getCommandType();
		virtual void execute();
		clock_t getIssueTime();
};

#endif