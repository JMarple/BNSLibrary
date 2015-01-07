// Gaussian.h
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/18/2014
//
// This header file includes code that
// implements Gaussian distributions for use in
// Vex Robotics Competition.  These are used for
// many probabilist algorithms and can tell you
// certainty of a measurement
//
// Source Code:
//   Gaussian.c
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

#ifndef __BNS_GAUSSIAN_H
#define __BNS_GAUSSIAN_H

#ifndef __BNS_MATRIX_H
#include "..\Matrix\BNSMatrix.h"
#endif

// Basic Gaussian Structure
struct Gaussian
{
  Matrix mean;
  Matrix cov;
};

// Function Prototype
float MultivariateNormalDistribution(Matrix data, Matrix mean, Matrix cov);

// GaussianInit(...)
// Initializes the gaussian distribution structure
void GaussianInit(Gaussian *gau, Matrix mean, Matrix cov);

// GaussianPDF(...)
// Produces the Probability Distribution Function of a given gaussian
float GaussianPDF(Gaussian *gau, Matrix data);

// FindMahalanobisDistance(...)
// Computes the mahalanobis distnance given a mean, covariance, and data point
// This differs from euclidean distance where the covariance will effect the
//  actual distance away from a datapoint.
float FindSquareMahalanobisDistance(Matrix data, Matrix mean, Matrix cov);
float FindMahalanobisDistance(Matrix data, Matrix mean, Matrix cov);

#endif
