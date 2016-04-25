//Standard libraries
#include <iostream>
#include <cstdlib>
#include <cstring>

//Third parties libraries


//Local libraries
#include "io_utils.h"

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

int main(int argc, char** argv)	{
	// check if there are more than 2 arguements
	if(argc<2)	{
		usage_message("Trim_Simulator-"+version+"[options] <Simulation Mode>\n");
	}

	//simulation mode variables
	bool sequential_trim=false;
	bool queued_trim=false;
	bool new_trim=false;

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
		}
	}//end for

	return 0;
}