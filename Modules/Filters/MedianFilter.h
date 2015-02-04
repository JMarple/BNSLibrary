// MedianFilter.h
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/18/2014
//
// This source file includes source code that
// implements a MedianFilter for use in
// Vex Robotics Competition.  What median filters
// do is take a set of data and determines the
// median of it.  This allows for spantenous noise
// that would otherwise negatively impact the data
// to be ignored.
//
// Source Code:
//   MedianFilter.c
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

#ifndef __BNS_MEDIAN_FILTER_H
#define __BNS_MEDIAN_FILTER_H

#ifndef __BNS_MATRIX_H
#include "..\Core\Matrix\BNSMatrix.h"
#endif

// Simple Median Filter structure for keeping track
//  of data
struct MedianFilter
{
  struct Matrix data;
};

void MedianInit(struct MedianFilter *filter, int size);
void MedianAddData(struct MedianFilter *filter, float data);
float MedianCompute(struct MedianFilter *filter);

#endif
