// BNSLib.h
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/18/2014
//
// This file implements all of the features
// of the BNS Library.
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

#ifndef __BNS_LIB_H
#define __BNS_LIB_H

// Compilers (make a new instance for your specific use case)
#define cROBOTC 0
#define cGCC    1

// --------------------------
// Set which compiler to use
#define COMPILER cROBOTC
// --------------------------

// ROBOTC compiling
#if COMPILER == cROBOTC

  // Core
  #include "Core\BNSHeap.c"
  #include "Core\BNSCore.c"
  #include "Matrix\BNSMatrix.c"
  #include "Matrix\BNSMatrixArithmetic.c"
  #include "Matrix\BNSMatrixScalar.c"
  #include "Filters\PIDController.c"
  #include "Core\BNSDataStructures.c"

  // Features
  #include "Probability\Gaussian.c"
  #include "Filters\KalmanFilter.c"
  #include "Filters\MedianFilter.c"

#elif COMPILER == cGCC

  #include <stdbool.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <time.h>

  // Rename several functions for the GCC compiler to interupt
  #define srand(nSysTime); time_t t;srand((unsigned)time(&t));
  #define writeDebugStreamLine printf("");printf
  #define writeDebugStream printf

  #include "Core\BNSCore.h"
  #include "Core\BNSHeap.h"
  #include "Matrix\BNSMatrix.h"

#endif

#endif
