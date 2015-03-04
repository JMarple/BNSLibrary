// MotionProfileExample.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 2/3/2015
//
// This example file shows how to use
// the Motion Profile built into the
// BNS Library
//
// Dependencies:
//   BNSLib.h in previous directory
//
// ------------------------------------------------------------------------
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// The author can be contacted by email at jmarple@umass.edu
//
// ------------------------------------------------------------------------
#include "..\BNSLib.h"

task main()
{
	// Initialize the BNS Library
	BNS();

	// Motion Profile object
	MotionProfile prof1;
	MotionProfileInit(&prof1);

	// Initiate the motion profile
	// It is very important that the paramters get set correctly,
	//  otherwise you'll likely get a "IMPOSSIBLE TO REACH LOCATION"
	//  error on your debug stream.
	// Be sure, if you initial velocity is LESS then your max velocity,
	//  to set the acceleration to positive, otherwise negative. Same goes
	//  for max to end velocity as well.  (Think, are you increasing in speed
	//  to reach the next velocity?  Then positive acceleration)
	// Be sure if you want your distance to be positive, the max velocity
	//  is set the same, positive.  And the opposite for negatives.
	// Use MotionProfilePrint(&prof1, #.##); to see the motion profile will
	//  execute on the debug stream
	//
	// ADVANCED PARAMS SETTING:
	//
	// Forward:
	// MotionProfileSetAccel(&prof1, 0.5, -1); // Accel = 0.5, decel = -1
	// MotionProfileSetDistance(&prof1, 1200); // Distance = 1200
	// MotionProfileSetVelocity(&prof1, 0, 15, 0); // V_0 = 0, V_max = 15, V_exit = 0
	//
	// Backward:
	// MotionProfileSetAccel(&prof1, -0.5, 1); // Accel = -0.5, decel = 1
	// MotionProfileSetDistance(&prof1, -1200); // Distance = -1200
	// MotionProfileSetVelocity(&prof1, 0, -15, 0); // V_0 = 0, V_max = -15, V_exit = 0
	//
	// SIMPLE PARAMS SETTING:
	//
	// Forward:
	MotionProfileEasyParams(&prof1, 1.0, 15.0, 1200.0); // Acceleration of 1, Velocity of 15, and distance of 1200
	//
	// Backward:
	// MotionProfileEasyParams(&prof1, 1.0, 15.0, -1200.0);
	//

	// "Execute" motion profile by printing the velocities to the screen
	float time = 0;
	float dt = 1; // Update rate: in this instance once per second

	while(!MotionProfileIsComplete(&prof1, time))
	{
		float velocity = MotionProfileCompute(&prof1, time);
		writeDebugStreamLine("%f", velocity);
		time+=dt;
	}

	writeDebugStreamLine("Done! :)");
}
