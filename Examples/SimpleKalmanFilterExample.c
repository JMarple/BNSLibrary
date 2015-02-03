// SimpleKalmanFilterExample.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 2/2/2014
//
// This example file shows how to use
// the kalman filter included in the
// BNSLibrary
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
  // Initiate BNS Library
  BNS();

  // Create a Simple KalmanFilter with an update every 0.1 seconds
  KalmanFilter filter1;
  SimpleKalmanInit(&filter1, 0.1);

  for(int i = 0; i <= 10; i++)
  {
  	// This inputs our data "i" and shows how
    //  the kalman filter reacts
  	float result = SimpleKalmanUpdate(&filter1, i);
  	writeDebugStreamLine("Result = %f", result);
  }
}
