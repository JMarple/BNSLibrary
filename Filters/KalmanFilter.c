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

#pragma systemFile

#ifndef __BNS_MATRIX_H
  #include "../Matrix/BNSMatrix.h"
#endif

struct KalmanFilter
{
  // Data output, n-length
  Matrix meanVector; // "x"	nx1 vector

  // Sensors measurements, m-length
  Matrix measurementVector; // "z" mx1 vector

  // Prediction
  Matrix updateMatrix; // "F" nxn matrix

  // What would we expect +the matrix to be if we
  //  got PERFECT data
  Matrix extractionMatrix; // "H" mxn matrix

  // Covariance of our data
  Matrix covarianceMatrixX; // "P" nxn matrix

  // Measurement Covariance Matrix
  Matrix covarianceMatrixZ; // "R" mxm matrix

  Matrix moveVector;
};

// Kalman filter initilizations
void KalmanInit(KalmanFilter kal, int variables, int measurements,
                      Matrix updateMatrix,
                      Matrix extractionMatrix,
                      Matrix covarianceMatrixX,
                      Matrix covarianceMatrixZ,
                      Matrix moveVector)
{
  CreateZerosMatrix(kal.meanVector, variables, 1);
  CreateZerosMatrix(kal.measurementVector, measurements, 1);

  CopyMatrix(kal.updateMatrix, updateMatrix);
  CopyMatrix(kal.extractionMatrix, extractionMatrix);
  CopyMatrix(kal.moveVector, moveVector);
  CopyMatrix(kal.covarianceMatrixX, covarianceMatrixX);
  CopyMatrix(kal.covarianceMatrixZ, covarianceMatrixZ);
}

// Kalman Filter Prediction Step
// We "predict" what the next data will be
//   What this means is the filter's mean will change
//   but we loose "certainty" about where the data is
//   (aka. the covariance will increase in this step)
void KalmanPredict(KalmanFilter kal)
{
  Matrix x_next;
  Matrix P_next;
  Matrix F_trans;

  // Calculate x_next (update guassian mean)
  MatrixMult(x_next, kal.updateMatrix, kal.meanVector);
  MatrixAdd(x_next, x_next, kal.moveVector);

  // Calculate p_next (update guassian covariance);
  MatrixMult(P_next, kal.updateMatrix, kal.covarianceMatrixX);
  MatrixTranspose(F_trans, kal.updateMatrix);
  MatrixMult(P_next, P_next, F_trans);

  // Copy results to the kalmanfilter class
  CopyMatrixByValue(kal.meanVector, x_next);
  CopyMatrixByValue(kal.covarianceMatrixX, P_next);

  DeleteMatrix(x_next);
  DeleteMatrix(P_next);
  DeleteMatrix(F_trans);
}

// Kalman Update Step
// We "update" given what we get for data
//   The filter will use the data given to lower our
//   uncertainty (aka. covariance)
void KalmanUpdate(KalmanFilter kal, Matrix meas)
{
  CopyMatrix(kal.measurementVector, meas);

  // Find the difference between the move (measurement)
  //   and what we predicted (extraction * data)
  Matrix y;
  MatrixMult(y, kal.extractionMatrix, kal.meanVector);
  MatrixSub(y, kal.measurementVector, y);

  // The Covariance of the move
  Matrix S;
  Matrix extTran;

  MatrixMult(S, kal.extractionMatrix, kal.covarianceMatrixX);
  MatrixTranspose(extTran, kal.extractionMatrix);
  MatrixMult(S, S, extTran);
  MatrixAdd(S, S, kal.covarianceMatrixZ);

  // Kalman Gain
  Matrix K;
  Matrix Sinv;

  MatrixInv(Sinv, S);
  MatrixMult(K, kal.covarianceMatrixX, extTran);
  MatrixMult(K, K, Sinv);

  // Figure out mean and covariance results
  Matrix x_next;
  Matrix P_next;

  MatrixMult(x_next, K, y);
  MatrixAdd(x_next, kal.meanVector, x_next);

  MatrixMult(P_next, kal.covarianceMatrixX, extTran);
  MatrixMult(P_next, P_next, Sinv);
  MatrixMult(P_next, P_next, kal.extractionMatrix);
  MatrixMult(P_next, P_next, kal.covarianceMatrixX);

  MatrixSub(P_next, kal.covarianceMatrixX, P_next);

  // Copy results to the kalmanfilter class
  CopyMatrixByValue(kal.meanVector, x_next);
  CopyMatrixByValue(kal.covarianceMatrixX, P_next);

  // Delete matricies so we don't have memory leaks..
  DeleteMatrix(y);
  DeleteMatrix(S);
  DeleteMatrix(extTran);
  DeleteMatrix(K);
  DeleteMatrix(Sinv);
  DeleteMatrix(x_next);
  DeleteMatrix(P_next);
}
