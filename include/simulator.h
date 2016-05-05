#ifndef SIMULATOR_H
	#define SIMULATOR_H
//Standard libraries


//Third parties libraries


//Local libraries


class Simulator	{
	private:
	protected:
		// maximum concurrent operation
		int maxOperation;
	public:
		Simulator();
		Simulator(int maxOp);
		~Simulator();
		virtual void startSimulation();
		void commandIssuer();
};
	
#endif