// RobotDataExample.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 05/31/2015
//
// This example file shows how
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
	BNS();

	// TODO: Finish this example 'n' stuff

	RobotData test;
	RobotDataInit(&test, 5);
	RobotDataUpdateVelocity(&test, 5.5);

	for(int i = 0; i < 5; i++)
	{
		float position = RobotDataGetPosition(&test, i);
		float velocity = RobotDataGetVelocity(&test, i);
		writeDebugStreamLine("At %d is %f with vel of %f", i, position, velocity);
	}

	RobotDataUpdateVelocity(&test, 3.14);

  for(int i = 0; i < 5; i++)
	{
		float position = RobotDataGetPosition(&test, i);
		float velocity = RobotDataGetVelocity(&test, i);
		writeDebugStreamLine("At %d is %f with vel of %f", i, position, velocity);
	}

  writeDebugStreamLine("SUM = %f", RobotDataGetPositionSummed(&test));

}
