#ifndef COMMAND_H
	#define COMMAND_H

//Standard Libraries
#include <iostream>
#include <time.h>

//Third Parties Libraries


//Local Includes
#include "macro.h"

//MACRO
// #define TRIM_COMMAND 'T';
// #define WRITE_COMMAND 'W';
// #define READ_COMMAND 'R';

class Command 	{
	private:
		double processTime;
		char type;
	protected:
		double issueTime;
	public:
		Command();
		Command(double issueTime, char type);
		~Command();
		double getIssueTime();
		// double getProcessTime();
		char getType();
};

#endif