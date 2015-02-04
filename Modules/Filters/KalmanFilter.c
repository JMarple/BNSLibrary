// KalmanFilter.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/18/2014
//
// This source file includes source code that
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

#pragma systemFile

#ifndef __BNS_KALMAN_FILTER_C
#define __BNS_KALMAN_FILTER_C

#ifndef __BNS_KALMAN_FILTER_H
#include "KalmanFilter.h"
#endif
#ifndef __BNS_MATRIX_H
#include "..\Core\Matrix\BNSMatrix.h"
#endif

// Kalman filter initilizations
void KalmanInit(struct KalmanFilter *kal, int variables, int measurements,
                      struct Matrix updateMatrix,
                      struct Matrix extractionMatrix,
                      struct Matrix covarianceMatrixX,
                      struct Matrix covarianceMatrixZ,
                      struct Matrix moveVector)
{
  CreateZerosMatrix(&kal->meanVector, variables, 1);
  CreateZerosMatrix(&kal->measurementVector, measurements, 1);

  CopyMatrix(&kal->updateMatrix, updateMatrix);
  CopyMatrix(&kal->extractionMatrix, extractionMatrix);
  CopyMatrix(&kal->moveVector, moveVector);
  CopyMatrix(&kal->covarianceMatrixX, covarianceMatrixX);
  CopyMatrix(&kal->covarianceMatrixZ, covarianceMatrixZ);
}

// Kalman Filter Prediction Step
// We "predict" what the next data will be
//   What this means is the filter's mean will change
//   but we loose "certainty" about where the data is
//   (aka. the covariance will increase in this step)
void KalmanPredict(struct KalmanFilter *kal)
{
  struct Matrix x_next, P_next, F_trans;

  CreateBlankMatrix(x_next);
  CreateBlankMatrix(P_next);
  CreateBlankMatrix(F_trans);

  // Calculate x_next (update guassian mean)
  MatrixMult(&x_next, kal->updateMatrix, kal->meanVector);
  MatrixAdd(&x_next, x_next, kal->moveVector);

  // Calculate p_next (update guassian covariance);
  MatrixMult(&P_next, kal->updateMatrix, kal->covarianceMatrixX);
  MatrixTranspose(&F_trans, kal->updateMatrix);
  MatrixMult(&P_next, P_next, F_trans);

  // Copy results to the kalmanfilter class
  CopyMatrixByValue(&kal->meanVector, x_next);
  CopyMatrixByValue(&kal->covarianceMatrixX, P_next);

  DeleteMatrix(&x_next);
  DeleteMatrix(&P_next);
  DeleteMatrix(&F_trans);
}

// Kalman Update Step
// We "update" given what we get for data
//   The filter will use the data given to lower our
//   uncertainty (aka. covariance)
void KalmanUpdate(struct KalmanFilter *kal, struct Matrix meas)
{
  struct Matrix y, S, extTran, K, Sinv, x_next, P_next;

  CreateBlankMatrix(y);
  CreateBlankMatrix(S);
  CreateBlankMatrix(extTran);
  CreateBlankMatrix(K);
  CreateBlankMatrix(Sinv);
  CreateBlankMatrix(x_next);
  CreateBlankMatrix(P_next);

  CopyMatrix(&kal->measurementVector, meas);

  // Find the difference between the move (measurement)
  //   and what we predicted (extraction * data)
  MatrixMult(&y, kal->extractionMatrix, kal->meanVector);
  MatrixSub(&y, kal->measurementVector, y);

  // The Covariance of the move
  MatrixMult(&S, kal->extractionMatrix, kal->covarianceMatrixX);
  MatrixTranspose(&extTran, kal->extractionMatrix);
  MatrixMult(&S, S, extTran);
  MatrixAdd(&S, S, kal->covarianceMatrixZ);

  // Kalman Gain
  MatrixInv(&Sinv, S);
  MatrixMult(&K, kal->covarianceMatrixX, extTran);
  MatrixMult(&K, K, Sinv);

  // Figure out mean and covariance results
  MatrixMult(&x_next, K, y);
  MatrixAdd(&x_next, kal->meanVector, x_next);

  MatrixMult(&P_next, kal->covarianceMatrixX, extTran);
  MatrixMult(&P_next, P_next, Sinv);
  MatrixMult(&P_next, P_next, kal->extractionMatrix);
  MatrixMult(&P_next, P_next, kal->covarianceMatrixX);

  MatrixSub(&P_next, kal->covarianceMatrixX, P_next);

  // Copy results to the kalmanfilter class
  CopyMatrixByValue(&kal->meanVector, x_next);
  CopyMatrixByValue(&kal->covarianceMatrixX, P_next);

  // Delete matricies so we don't have memory leaks..
  DeleteMatrix(&y);
  DeleteMatrix(&S);
  DeleteMatrix(&extTran);
  DeleteMatrix(&K);
  DeleteMatrix(&Sinv);
  DeleteMatrix(&x_next);
  DeleteMatrix(&P_next);
}

#endif
