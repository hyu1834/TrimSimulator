#ifndef SEQUENTIAL_TRIM_H
	#define SEQUENTIAL_TRIM_H
//Standard libraries


//Third parties libraries


//Local libraries
#include "base_trim.h"


class Sequential_Trim: public Trim	{
	private:
	protected:
	public:
		Sequential_Trim();
		~Sequential_Trim();
		void startSimulation();
};
	
#endif