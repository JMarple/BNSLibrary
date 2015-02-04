// KalmanFilter.h
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/18/2014
//
// This header file includes code that
// implements a KalmanFilter for use in
// Vex Robotics Competition.  There is some
// explanation about how the kalman filter works
// in the examples folder, however some previous
// experience/intuition may be necessary to
// succesfully integrate it.
//
// Source Code:
//   KalmanFilter.c
//
// Dependencies:
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


#ifndef __BNS_KALMAN_FILTER_H
#define __BNS_KALMAN_FILTER_H

#ifndef __BNS_MATRIX_H
#include "..\Core\Matrix\BNSMatrix.h"
#endif

struct KalmanFilter
{
  // Data output, n-length
  struct Matrix meanVector; // "x"	nx1 vector

  // Sensors measurements, m-length
  struct Matrix measurementVector; // "z" mx1 vector

  // Prediction
  struct Matrix updateMatrix; // "F" nxn matrix

  // What would we expect +the matrix to be if we
  //  got PERFECT data
  struct Matrix extractionMatrix; // "H" mxn matrix

  // Covariance of our data
  struct Matrix covarianceMatrixX; // "P" nxn matrix

  // Measurement Covariance Matrix
  struct Matrix covarianceMatrixZ; // "R" mxm matrix

  struct Matrix moveVector;
};

// Kalman filter initilizations
void KalmanInit(struct KalmanFilter *kal, int variables, int measurements,
                      struct Matrix updateMatrix,
                      struct Matrix extractionMatrix,
                      struct Matrix covarianceMatrixX,
                      struct Matrix covarianceMatrixZ,
                      struct Matrix moveVector);

// Kalman Filter Prediction Step
// We "predict" what the next data will be
//   What this means is the filter's mean will change
//   but we loose "certainty" about where the data is
//   (aka. the covariance will increase in this step)
void KalmanPredict(struct KalmanFilter *kal);

// Kalman Update Step
// We "update" given what we get for data
//   The filter will use the data given to lower our
//   uncertainty (aka. covariance)
void KalmanUpdate(struct KalmanFilter *kal, struct Matrix meas);

#endif
