//Standard libraries
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <vector>

//Third parties libraries

//Local Includes
#include "macro.h"
#include "io_utils.h"

#include "command.h"
#include "trim_command.h"
#include "io_command.h"

#include "sequential_trim_simulator.h"
#include "queued_trim_simulator.h"
#include "io_trim_simulator.h"
#include "semi_queued_trim_simulator.h"
#include "hold_trim_simulator.h"



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
				"\t-a, --activity\n\t\t-a <activity files>\n\n"
				"\t-s, --statistic\n\t\t-s <statistic files>\n\n"
				"\t-c, --concurrent\n\t\t-c <number of concurrent commands>\n\n"
				"\t-sq, --sequential\n\t\tsequential simulation\n\n"
				"\t-q, --queued\n\t\tqueued simulation\n\n"
				"\t-it, --io_trim\n\t\tio trim simulation\n\n"
				"\t-sem, --semi_queue\n\t\tsemi-queued simulation\n\n"
				"\t-ht, --hold_trim\n\t\thold trim simulation\n\n"
				"EXAMPLE\n"
				"\t./bin/TS -a <activity_filepath> -s <statistic_filepath> -c <number of concurrent commands> <simulation modes>\n"
				"\t./bin/TS -a test_case/activity_gen6.csv -s test_case/statistic_gen2.csv -c 2 -sq \n"
				"\t./bin/TS -a test_case/activity_gen6.csv -s test_case/statistic_gen2.csv -c 2 -q \n"
				"\t./bin/TS -a test_case/activity_gen6.csv -s test_case/statistic_gen2.csv -c 2 -sem \n"
				"\t./bin/TS -a test_case/activity_gen6.csv -s test_case/statistic_gen2.csv -c 2 -it \n"
				"\t./bin/TS -a test_case/activity_gen6.csv -s test_case/statistic_gen2.csv -c 2 -ht \n"
				"\t./bin/TS -a test_case/activity_gen6.csv -s test_case/statistic_gen2.csv -c 2 -sq -q -sem -it -ht\n"
				;
}//end printHelp

void praseStatistic(std::string statisticFilePath, double& readProcessTime, 
					double& writeProcessTime, double& trimProcessTime)	{
	// open the input file
	std::ifstream filePtr(statisticFilePath.c_str(), std::ifstream::in);
	if(filePtr)	{
		char commandType;
		double time;
		while(filePtr >> commandType >> time)	{
			if(commandType == 'T')
				trimProcessTime = time;
			else if(commandType == 'W')
				writeProcessTime = time;
			else if(commandType == 'R')
				readProcessTime = time;
		}
	}
}

void procedurePraser(std::string procedureFilePath, std::vector<Command*>& procedure)	{
	// open the input file
	std::ifstream filePtr(procedureFilePath.c_str(), std::ifstream::in);
	// if open successfully
	if(filePtr)	{
		double issueTime;
		char commandType;
		long startingBlock;
		long nextNBlock;
		// read everyline
		while(filePtr >> issueTime >> commandType >> startingBlock >> nextNBlock)	{
			// create command based on command type
			if(commandType == 'T')	{
				procedure.push_back(new Trim_Command(issueTime, startingBlock, nextNBlock));
			}
			else	{
				procedure.push_back(new IO_Command(issueTime, commandType, startingBlock, nextNBlock));
			}
		}
	}
}

int main(int argc, char** argv)	{
	// check if there are more than 2 arguements
	if(argc<2)	{
		usage_message("Trim_Simulator-"+version+"[options] <Simulation Mode>\n");
	}

	//input file path variables
	std::string statisticFilePath = "";
	std::string procedureFilePath = "";

	// simulation statistic variables
	double readProcessTime = -1;
	double writeProcessTime = -1;
	double trimProcessTime = -1;

	//simulation variables
	int concurrent_command = 1;
	bool sequential_trim_simulation = false;
	bool queued_trim_simulation = false;
	bool io_trim_simulation = false;
	bool semi_queue_trim_simulation = false;
	bool hold_trim_simulation = false;

	/*Parse the command line and load all input files*/
	for(int i=1;i<argc;i++)	{
		if(argv[i][0]=='-')	{
			if((strcasecmp(argv[i]+1,"help")==0)||(strcasecmp(argv[i]+1,"h")==0)||(strcasecmp(argv[i]+1,"-help")==0))	{
				printHelp();
				return 0;
			}//end if strcasecmp
			else if((strcasecmp(argv[i]+1,"s")==0)|| (strcasecmp(argv[i]+1,"-statistic")==0))	{
				++i;
				if(i<argc)	{
					statisticFilePath = argv[i];
				}//end if
				else
					std::cerr<<"Dangling -s or --statistic flag on command line\n";
			}
			else if((strcasecmp(argv[i]+1,"a")==0)|| (strcasecmp(argv[i]+1,"-activity")==0))	{
				++i;
				if(i<argc)	{
					procedureFilePath = argv[i];
				}//end if
				else
					std::cerr<<"Dangling -a or --activity flag on command line\n";
			}
			else if((strcasecmp(argv[i]+1,"c")==0)|| (strcasecmp(argv[i]+1,"-concurrent")==0))	{
				++i;
				if(i<argc)	{
					concurrent_command = atoi(argv[i]);
				}//end if
				else
					std::cerr<<"Dangling -c or --concurrent flag on command line\n";
			}

			else if((strcasecmp(argv[i]+1,"sq")==0)|| (strcasecmp(argv[i]+1,"-sequential")==0))	{
				sequential_trim_simulation = true;
			}//end else if
			else if((strcasecmp(argv[i]+1,"q")==0)|| (strcasecmp(argv[i]+1,"-queued")==0))	{
				queued_trim_simulation = true;
			}//end else if
			else if((strcasecmp(argv[i]+1,"it")==0)|| (strcasecmp(argv[i]+1,"-io_trim")==0))	{
				io_trim_simulation = true;
			}//end else if
			else if((strcasecmp(argv[i]+1,"sem")==0)|| (strcasecmp(argv[i]+1,"-semi_queue")==0))	{
				semi_queue_trim_simulation = true;
			}//end else if
			else if((strcasecmp(argv[i]+1,"ht")==0)|| (strcasecmp(argv[i]+1,"-hold_trim")==0))	{
				hold_trim_simulation = true;
			}//end else if
			else	{
				std::cerr<<"Error: Unrecognized option - "<<argv[i]<<"\n";
			}
		}
		else	{
			std::cerr<<"Error: Unrecognized option - "<<argv[i]<<"\n";
		}
	}//end for

	if(statisticFilePath == "" || procedureFilePath == "")	{
		printHelp();
		return -1;
	}

	//simulation process code
	std::vector<Command*> commands;
	praseStatistic(statisticFilePath, readProcessTime, writeProcessTime, trimProcessTime);
	procedurePraser(procedureFilePath, commands);
	if(readProcessTime == -1 || writeProcessTime == -1 || trimProcessTime == -1 || commands.size() < 1)	{
		printHelp();
		return -1;
	}

		

	if(sequential_trim_simulation)	{
		Sequential_Trim_Simulator* sts = new Sequential_Trim_Simulator(commands, concurrent_command);
		sts->startSimulation(readProcessTime, writeProcessTime, trimProcessTime);
		delete sts;
	}

	if(queued_trim_simulation)	{
		Queued_Trim_Simulator* qts = new Queued_Trim_Simulator(commands, concurrent_command);
		qts->startSimulation(readProcessTime, writeProcessTime, trimProcessTime);
		delete qts;
	}

	if(io_trim_simulation)	{
		IO_Trim_Simulator* iots = new IO_Trim_Simulator(commands, concurrent_command);
		iots->startSimulation(readProcessTime, writeProcessTime, trimProcessTime);
		delete iots;
	}

	if(semi_queue_trim_simulation)	{
		Semi_Queued_Trim_Simulator* sqts = new Semi_Queued_Trim_Simulator(commands, concurrent_command);
		sqts->startSimulation(readProcessTime, writeProcessTime, trimProcessTime);
		delete sqts;
	}

	if(hold_trim_simulation)	{
		Hold_Trim_Simulator* hts = new Hold_Trim_Simulator(commands, concurrent_command);
		hts->startSimulation(readProcessTime, writeProcessTime, trimProcessTime);
		delete hts;
	}


	/*
		clean up
	*/
	// delete all command pointers
	for(int i = 0; i < commands.size(); i++)	{
		delete commands[i];
	}
	commands.clear();

	return 0;
}






