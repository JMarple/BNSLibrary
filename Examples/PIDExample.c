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

  // The PID controller that will be used
  // The basic usage of this PID controller is as following:
  //
  // PID pid1;
  // PIDInit(&pid1, PConstant, IConstant, DConstant);
  // float feedback = PIDCompute(&pid1, your_error);
  //
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
    motorSpeed = pidResult;

    // This "simulates" our robot's dynamics, not necessary for a real robot
    motorSpeed += momentum;
    momentum += pidResult * kMomentum;
    writeDebugStreamLine("%f", motorSpeed);

    // There is a bug in RobotC where if you print too fast,
    //   you might get weird characters at random
    delay(1);
  }
}
