// Gaussian.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/18/2014
//
// This source file includes source code that
// implements Gaussian distributions for use in
// Vex Robotics Competition.  These are used for
// many probabilist algorithms and can tell you
// certainty of a measurement
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

#ifndef __BNS_GAUSSIAN_C
#define __BNS_GAUSSIAN_C

#ifndef __BNS_MATRIX_H
#include "../Matrix/BNSMatrix.h"
#endif
#ifndef __BNS_GAUSSIAN_H
#include "BNSGaussian.h"
#endif

// Initialize basic values for a gaussian
void GaussianInit(Gaussian *gau, Matrix mean, Matrix cov)
{
  DeleteMatrix(&gau->mean);
  DeleteMatrix(&gau->cov);
  CopyMatrixByValue(&gau->mean, mean);
  CopyMatrixByValue(&gau->cov, cov);
}

// Calculate the probabilty distribution function
float GaussianPDF(Gaussian *gau, Matrix data)
{
  return MultivariateNormalDistribution(data, gau->mean, gau->cov);
}

// Mahalanobis Distance, good for finding distances within a guassian
float FindSquareMahalanobisDistance(Matrix data, Matrix mean, Matrix cov)
{
  Matrix sub1;
  Matrix sub2;
  Matrix result;
  Matrix inverse;
  CreateZerosMatrix(sub1, data.m, 1);
  CreateZerosMatrix(sub2, 1, data.m);
  CreateZerosMatrix(inverse, cov.m, cov.n);
  CreateZerosMatrix(result, 1, 1);

  // (x-u)^T*cov^-1*(x-u)

  // Subtract the data from the mean to find an error
  MatrixSub(sub1, data, mean);

  // Tranpose vector
  MatrixTranspose(sub1, sub1);

  // Find inverse, it it doesn't exist, there is an issue!
  if( !MatrixInv(inverse, cov) )
    return 0;

  // Multiply the Tranposed "error" and the inverse of covarience
  MatrixMult(sub2, sub1, inverse);

  // Go back to original vector
  MatrixTranspose(sub1, sub1);

  MatrixMult(result, sub2, sub1);

  float finalResult = GetMatrixAt(result, 0, 0);

  DeleteMatrix(sub1);
  DeleteMatrix(sub2);
  DeleteMatrix(inverse);
  DeleteMatrix(result);

  return finalResult;
}

float FindMahalanobisDistance(Matrix data, Matrix mean, Matrix cov)
{
  return sqrt(FindSquareMahalanobisDistance(data, mean, cov));
}

// Multivariate Normal Guassian Distribution
//  Returns the PDF of a set of data, given a guassian mean and covariance
//  See the example for more details.
float MultivariateNormalDistribution(Matrix data, Matrix mean, Matrix cov)
{
  float mahDist = -0.5 * FindSquareMahalanobisDistance(data, mean, cov);

  float normalizer = sqrt( pow(6.28318, data.m) * MatrixDeterminant(cov) );

  return 1 / normalizer * exp(mahDist);
}

#endif
