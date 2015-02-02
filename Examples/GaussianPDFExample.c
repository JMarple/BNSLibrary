// GaussianPDFExample.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/16/2014
//
// This example file shows how to use
// the multivariate PDF included in the
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

  // Our input data
  Matrix data;
  CreateMatrix(&data, "1.1; 10.1; 0");

  // Guassian Man
  // Ie. Where is the guassisan located?
  Matrix mean;
  CreateMatrix(&mean, "1; 10; 0");

  // Guassian covariance
  // How "good" is our mean?
  // A low number like 0.1 means we're very certain
  // A high number like 1000 means we have little knowledge
  //  of the "true" mean
  Matrix cov;
  CreateMatrix(&cov, "0.1 0 0; 0 0.1 0; 0 0 0.1;");

  // Setup our gaussian
  Gaussian gau;
  GaussianInit(&gau, mean, cov);

  // Print Result of the PDF function
  // Expected to be 1.816775
  float result = GaussianPDF(&gau, data);
  writeDebugStreamLine("Result = %f", result);
}
