// PIDExample.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 01/07/2015
//
// This example file shows how to use
// the PID Controller built into the
// BNS Library
//
// Dependencies:
//   BNSLib.h in previous directory
#include "..\BNSLib.h"

task main()
{
	// Initialize the BNS Library
	BNS();

	// The PID controller that will be used
	PID pid1;
	PIDInit(&pid1, 0.1, 0, 0.1); // Set P, I, and D constants

	// This helps simulate a fake robot for the purpose of this demo
	float momentum = 0;
	float kMomentum = 0.2;

	// We start at 0 units and want to reach 100 units
	float motorSpeed = 0;
	float targetMotorSpeed = 100;

	// Output instructions to view the PID response
	writeDebugStreamLine("*** Copy/paste all the results in the debug window to Excel and graph what the PID response looks like! ***");

	// Loop through many times so we can graph
	//  the PID loop
	for(int i = 0; i < 200; i++)
	{
		// This calculates how far off we are from the true value
	  //  The PID will return a response that will hopefully minimize this error over time
		float pidResult = PIDCompute(&pid1, targetMotorSpeed - motorSpeed);

		// Add pid to motor value
		motorSpeed += pidResult;

		// This "simulates" our robot's dynamics, not necessary for a real robot
		motorSpeed += momentum;
		momentum += pidResult * kMomentum;
		writeDebugStreamLine("%f", motorSpeed);
	}
}
