#ifndef COMMAND_H
	#define COMMAND_H

//Standard Libraries
#include <iostream>
#include <time.h>

//Third Parties Libraries


//Local Includes
#include "macro.h"

//MACRO
#define TRIM_COMMAND 'T';
#define WRITE_COMMAND 'W';
#define READ_COMMAND 'R';

class Command 	{
	private:
		float processTime;
		char type;
	protected:
		float issueTime;
	public:
		Command();
		Command(float issueTime, char type);
		~Command();
		virtual int getCommandType();
		virtual void execute();
		float getIssueTime();
		float getProcessTime();
		char getType();
};

#endif