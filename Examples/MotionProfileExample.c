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

  // Motion Profile Constants
  MotionProfile prof1;

  // Initiate the motion profile with a distance of 0
  MotionProfileInit(&prof1);
  MotionProfileSetAccel(&prof1, -0.5, 1); // Accel = 0.5, decel = 1
  MotionProfileSetDistance(&prof1, -1000); // Distance = 1000
  MotionProfileSetVelocity(&prof1, 6, -15, -2); // V_0 = 0, V_max = 25, V_exit = 0

  // "Execute" motion profile by printing the velocities to the screen
  float time = 0;
  float dt = 1; // Update rate: in this instance once per second
  while(!MotionProfileIsComplete(&prof1, time))
	{
  	float velocity = MotionProfileCompute(&prof1, time);
  	writeDebugStreamLine("%f", velocity);
		time+=dt;
		delay(2);
  }

  writeDebugStreamLine("Done! :)");
}
