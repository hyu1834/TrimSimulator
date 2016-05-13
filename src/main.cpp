//Standard libraries
#include <iostream>
#include <cstdlib>
#include <cstring>

//Third parties libraries


//Local Includes
#include "io_utils.h"
#include "sequential_trim_simulator.h"
#include "queued_trim_simulator.h"


std::string version="0.0.1";

void printHelp()	{
	//std::clog<< "\nNAME\n"
	//			"\tTS - Trim_Simulator-"<<version<<"\n\n"
	//			"SYNOPSIS\n"
	//			"\tTS [OPTION]... DIRECTORY\n"
	//			"\n"
	//			"DESCRIPTION\n"
	//			"\tLinux Trim Simulator\n\n"
	//			"\t-help, --help\n\t\tTrim Simulator Description\n\n"
	//			;
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
		//if(argv[i][0]=='-')	{
		//	if((strcasecmp(argv[i]+1,"help")==0)||(strcasecmp(argv[i]+1,"h")==0)||(strcasecmp(argv[i]+1,"-help")==0))	{
		//		printHelp();
		//		return 0;
		//	}//end if strcasecmp
		//	else if((strcasecmp(argv[i]+1,"s")==0)|| (strcasecmp(argv[i]+1,"-sequential")==0))	{
		//		sequential_trim=true;
		//		// ++i;
		//		// if(i<argc)	{
		//		// 	inFilePath[1] = verifyDirectoryPath(argv[i]);
		//		// }//end if
		//		// else
		//		// 	std::cerr<<"Dangling -l or --lasfdata flag on command line\n";
		//	}//end else if
		//	else if((strcasecmp(argv[i]+1,"q")==0)|| (strcasecmp(argv[i]+1,"-queued")==0))	{
		//		queued_trim=true;
		//	}//end else if
		//	else if((strcasecmp(argv[i]+1,"n")==0)|| (strcasecmp(argv[i]+1,"-new_trim")==0))	{
		//		new_trim=true;
		//	}//end else if
		//}
	}//end for

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






