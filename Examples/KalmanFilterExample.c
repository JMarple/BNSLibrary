// KalmanFilterExample.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/16/2014
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

  // Kalman Filter Example:
  //  We will input 2 data points which are independent "positions"
  //  Then we will want 4 outputs, a position estimations and then a
  //    velocity estimation for each data point.
  //
  // So, input "SensorPos1" into our Kalman filter, we will get out
  //   estimated "KalmanOutputPosition1" and "KalmanOutputVelocity1"

  // Kinematic Relationship Matrix
  // This tells our kalman filter how the 4 outputs relate
  // For instance, remember back to physics.  You have the following equation
  //   new_position = position + velocity * time
  // We can do this in our filter, where x1 is our position for data1,
  //   x2 is the velocity for data1, x3 is position for data2, and x4 is
  //   velocity for x4.
  //  x1' = x1 + x2*dt
  //  x2' = x2
  //  x3' = x3 + x4*dt
  //  x4' = x4
  //
  // Or in matrix form:
  // [1, dt, 0, 0 ]
  // [0, 1,  0, 0 ]
  // [0, 0,  1, dt]
  // [0, 0,  0, 1 ]
  float dt = 0.1;
  Matrix updateMatrix;
  CreateZerosMatrix(updateMatrix, 4, 4);
  SetMatrixAt(updateMatrix, 0, 0, 1);
  SetMatrixAt(updateMatrix, 0, 1, dt);
  SetMatrixAt(updateMatrix, 1, 0, 0);
  SetMatrixAt(updateMatrix, 1, 1, 1);
  SetMatrixAt(updateMatrix, 2, 2, 1);
  SetMatrixAt(updateMatrix, 2, 3, dt);
  SetMatrixAt(updateMatrix, 3, 2, 0);
  SetMatrixAt(updateMatrix, 3, 3, 1);

  // Extraction Matrix
  // This matrix relates our input to output
  // It tells us what we'd expect if we got PERFECT
  //   sensor data
  // So we'd expect position, x1 & x3, to be exactly what
  //   our sensors tell us
  Matrix extractionMatrix;
  CreateMatrix(extractionMatrix,
            "1 0 0 0;
             0 0 1 0");

  // Covariance of our output
  // This simply tell us how "sure" we are of our estimations
  // The main digonal tells us for each x1, x2, x3, x4 how good
  //   each estimation is.  If they are 0, we have perfect results,
  //   if we have high numbers, like 1000, we have very unsure results
  // Note: This will naturally converge over time, so it's a good idea to
  //   keep the variance very high at the start
  Matrix covarianceMatrixX;
  CreateMatrix(covarianceMatrixX,
            "1000 0 0 0;
            0 1000 0 0;
            0 0 1000 0;
            0 0 0 1000");

  // Covariance of our sensor data
  // How "good" is our sensor information
  // This does not converge over time, so it's important you tune to a fair number
  Matrix covarianceMatrixZ;
  CreateMatrix(covarianceMatrixZ, "1 0; 0 1");

  // Move Vector
  // The control input we know is happening.  Typically left at 0
  Matrix moveVector;
  CreateZerosMatrix(moveVector, 4, 1);

  // Our data we will update into the filter :)
  Matrix data;
  CreateZerosMatrix(data, 2, 1);
  SetMatrixAt(data, 0, 0, 1);
  SetMatrixAt(data, 1, 0, 2);

  // Create a new kalman filter and input our matricies
  KalmanFilter filter;
  KalmanInit(filter, 4, 2,
                    updateMatrix,
                    extractionMatrix,
                    covarianceMatrixX,
                    covarianceMatrixZ,
                    moveVector
                  );

  writeDebugStreamLine("Starting Kalman Filter Function:");

  // This is where we will input our "data" into the filter
  // For this example, we are giving "i" and "i*2" as an input position
  // So we expect our position to be "20" and "40", with the velocity to be
  //   1/dt = "10" and 2/dt = "20"
  // Remember i = position, velocity is how much we're incrementing our i each loop!
  int endPosition = 20;
  int velocity = 1;
  for(int i = 1; i <= endPosition; i+=velocity)
  {
    // "Fake" sensor noise, between -0.5 and 0.5
    float noise = (rand() % 1000)/1000.0 - 0.5;

    // Update Data Matrix
    SetMatrixAt(data, 0, 0, i + noise);
    SetMatrixAt(data, 1, 0, i*2 + noise);

    // Predict and Update Kalman Filter
    writeDebugStreamLine("Iteration %d Predict", i);
    KalmanPredict(&filter);
    writeDebugStreamLine("Iteration %d Update", i);
	  KalmanUpdate(&filter, data);
  }

  // Output estimation 1
  writeDebugStreamLine("Pos1 = %f, Vel1 = %f",
  GetMatrixAt(filter.meanVector, 0, 0),
  GetMatrixAt(filter.meanVector, 1, 0));

  // Output estimation 2
  writeDebugStreamLine("Pos2 = %f, Vel2 = %f",
  GetMatrixAt(filter.meanVector, 2, 0),
  GetMatrixAt(filter.meanVector, 3, 0));

  while(1==1);
}
