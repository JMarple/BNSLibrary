// MedianFilter.c
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

#pragma systemFile

#ifndef __BNS_MEDIAN_FILTER_C
#define __BNS_MEDIAN_FILTER_C

#ifndef __BNS_MEDIAN_FILTER_H
#include "MedianFilter.h"
#endif
#ifndef __BNS_MATRIX_H
#include "..\Core\Matrix\BNSMatrix.h"
#endif

// Initialize the filter's data vector
void MedianInit(struct MedianFilter *filter, int size)
{
  DeleteMatrix(&filter->data);
  CreateZerosMatrix(&filter->data, size, 1);
}

// Add data to the filter, push the oldest data out of the vector
void MedianAddData(struct MedianFilter *filter, float data)
{
  int i;
  for(i = filter->data.m-1; i > 0; i--)
    SetMatrixAt(&filter->data, i, 0, GetMatrixAt(&filter->data, i-1, 0));

  SetMatrixAt(&filter->data, 0, 0, data);
}

// Compute the median of the given data
float MedianCompute(struct MedianFilter *filter)
{
  struct Matrix sorted;
  int i, j, lowestIndex, middle;
  float tmpValue, returnValue;

  CopyMatrixByValue(&sorted, filter->data);

  // Selection Sort
  for(i = 0; i < sorted.m; i++)
  {
    lowestIndex = i;
    for(j = i+1; j < sorted.m; j++)
    {
      if(GetMatrixAt(&sorted, j, 0) < GetMatrixAt(&sorted, lowestIndex, 0))
          lowestIndex = j;
    }
    tmpValue = GetMatrixAt(&sorted, i, 0);
    SetMatrixAt(&sorted, i, 0, GetMatrixAt(&sorted, lowestIndex, 0));
    SetMatrixAt(&sorted, lowestIndex, 0, tmpValue);
  }

  // Get Median
  middle = sorted.m / 2;
  returnValue = GetMatrixAt(&sorted, middle, 0);

  // Prevent Memory Leaks
  DeleteMatrix(&sorted);

  // Return the median
  return returnValue;
}

#endif
