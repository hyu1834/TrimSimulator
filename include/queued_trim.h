#ifndef QUEUED_TRIM_H
	#define QUEUED_TRIM_H
//Standard libraries


//Third parties libraries


//Local libraries
#include "base_trim.h"


class Queued_Trim: public Trim	{
	private:
	protected:
	public:
		Queued_Trim();
		~Queued_Trim();
		void startSimulation();
};
	
#endif