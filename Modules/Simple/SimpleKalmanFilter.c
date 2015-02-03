// SimpleKalmanFilter.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 2/2/2014
//
// This source file includes source code that
// implements a KalmanFilter for use in
// Vex Robotics Competition.  This is a simple
// version of the original Kalman Filter code that
// assumes a basic update and extraction matrix
//
// updateMatrix = [[x1, x2*dt],[0, x2]]
// extractionMatrix = [[1, 0]]
//
// Essentially, you input sensor data and this will
//   output an approximation for position.
// Note: This will generally only improve performance
//   if noise is linear and gaussian!
//
// Source Code:
//   SimpleKalmanFilter.c
//
// Dependencies:
//    KalmanFilter.h
//    BNSMatrix.h
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

#pragma systemFile

#ifndef __BNS_SIMPLE_KALMAN_FILTER_C
#define __BNS_SIMPLE_KALMAN_FILTER_C

#ifndef __BNS_SIMPLE_KALMAN_FILTER_H
#include "SimpleKalmanFilter.h"
#endif

#ifndef __BNS_MATRIX_H
#include "..\Matrix\BNSMatrix.h"
#endif

// dt is the refresh rate in seconds per update
bool SimpleKalmanInit(struct KalmanFilter *filter, double dt)
{
	// Update Matrix
	struct Matrix updateMatrix;
	CreateZerosMatrix(&updateMatrix, 2, 2);
	SetMatrixAt(&updateMatrix, 0, 0, 1);
	SetMatrixAt(&updateMatrix, 0, 1, dt);
	SetMatrixAt(&updateMatrix, 1, 0, 0);
	SetMatrixAt(&updateMatrix, 1, 1, 1);

	// Extraction Matrix
	struct Matrix extractionMatrix;
  CreateMatrix(&extractionMatrix,"1 0");

  // Covariance Matrix for output
  struct Matrix covarianceMatrixX;
  CreateMatrix(&covarianceMatrixX,
            "1000 0;
            0 1000");

  // Covariance Matrix for sensor data
  struct Matrix covarianceMatrixZ;
  CreateMatrix(&covarianceMatrixZ, "1");

  // Move Vector control input
  struct Matrix moveVector;
  CreateZerosMatrix(&moveVector, 2, 1);

  // Initialize Kalman Filter
  KalmanInit(filter, 2, 1,
                    updateMatrix,
                    extractionMatrix,
                    covarianceMatrixX,
                    covarianceMatrixZ,
                    moveVector
                  );
  return true;
}

// Updates the kalman filter given sensor data
float SimpleKalmanUpdate(struct KalmanFilter* filter, float data)
{
	Matrix thisData;
	CreateZerosMatrix(&thisData, 1, 1);
	SetMatrixAt(&thisData, 0, 0, data);

	KalmanPredict(filter);
	KalmanUpdate(filter, thisData);

	DeleteMatrix(&thisData);

	return SimpleKalmanGetPosition(filter);
}

// Returns the most recent position value
float SimpleKalmanGetPosition(struct KalmanFilter* filter)
{
	return GetMatrixAt(&filter->meanVector, 0, 0);
}

// Returns the most recent velocity value
float SimpleKalmanGetVelocity(struct KalmanFilter* filter)
{
	return GetMatrixAt(&filter->meanVector, 1, 0);
}

#endif
