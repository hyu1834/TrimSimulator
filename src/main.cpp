//Standard libraries
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <vector>

//Third parties libraries


//Local Includes
#include "io_utils.h"
#include "sequential_trim_simulator.h"
#include "queued_trim_simulator.h"
#include "command.h"
#include "trim_command.h"
#include "io_command.h"


std::string version="0.0.1";

void printHelp()	{
	std::clog<< "\nNAME\n"
				"\tTS - Trim_Simulator-"<<version<<"\n\n"
				"SYNOPSIS\n"
				"\tTS [OPTION]... DIRECTORY\n"
				"\n"
				"DESCRIPTION\n"
				"\tLinux Trim Simulator\n\n"
				"\t-help, --help\n\t\tTrim Simulator Description\n\n"
				;
}//end printHelp

std::vector<Command*> procedurePraser(std::string procedureFilePath)	{
	// vector of command based on the input file
	std::vector<Command*> procedure;
	// open the input file
	std::ifstream commandFilePtr(procedureFilePath.c_str(), std::ifstream::in);
	// if open successfully
	if(commandFilePtr)	{
		float issueTime;
		char commandType;
		// read everyline
		while(commandFilePtr >> issueTime >> commandType)	{
			// create command based on command type
			if(commandType == 'T')	{
				procedure.push_back(new Trim_Command(issueTime));
			}
			else	{
				procedure.push_back(new IO_Command(issueTime, commandType));
			}
		}
	}

	return procedure;
}

int main(int argc, char** argv)	{
	// check if there are more than 2 arguements
	if(argc<2)	{
		usage_message("Trim_Simulator-"+version+"[options] <Simulation Mode>\n");
	}

	//simulation mode variables
	bool sequential_trim=false;
	bool queued_trim=false;
	bool new_trim=false;

	std::string procedureFilePath = "";

	/*Parse the command line and load all input files*/
	for(int i=1;i<argc;i++)	{
		if(argv[i][0]=='-')	{
			if((strcasecmp(argv[i]+1,"help")==0)||(strcasecmp(argv[i]+1,"h")==0)||(strcasecmp(argv[i]+1,"-help")==0))	{
				printHelp();
				return 0;
			}//end if strcasecmp
			else if((strcasecmp(argv[i]+1,"s")==0)|| (strcasecmp(argv[i]+1,"-sequential")==0))	{
				sequential_trim=true;
			}//end else if
			else if((strcasecmp(argv[i]+1,"q")==0)|| (strcasecmp(argv[i]+1,"-queued")==0))	{
				queued_trim=true;
			}//end else if
			else if((strcasecmp(argv[i]+1,"n")==0)|| (strcasecmp(argv[i]+1,"-new_trim")==0))	{
				new_trim=true;
			}//end else if
			else if((strcasecmp(argv[i]+1,"p")==0)|| (strcasecmp(argv[i]+1,"-procedure")==0))	{
				++i;
				if(i<argc)	{
					procedureFilePath = argv[i];
				}//end if
				else
					std::cerr<<"Dangling -p or --procedure flag on command line\n";
			}
		}
	}//end for

	//simulation process code
	std::vector<Command*> commands;

	if(procedureFilePath == "")	{
		commands = procedurePraser(procedureFilePath);
	}

	if(sequential_trim)	{
		Sequential_Trim_Simulator* sts = new Sequential_Trim_Simulator(2);
		sts->startSimulation();
	}

	if(queued_trim)	{
		Queued_Trim_Simulator* qts = new Queued_Trim_Simulator(2);
		qts->startSimulation();
	}

	if(new_trim)	{

	}

	return 0;
}






