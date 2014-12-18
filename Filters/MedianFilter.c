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

#pragma systemFile

#ifndef __BNS_MATRIX_H
  #include "../Matrix/BNSMatrix.h"
#endif

// Simple Median Filter structure for keeping track
//  of data
struct MedianFilter
{
  Matrix data;
};

// Initialize the filter's data vector
void MedianInit(MedianFilter filter, int size)
{
  DeleteMatrix(filter.data);
  CreateZerosMatrix(filter.data, size, 1);
}

// Add data to the filter, push the oldest data out of the vector
void MedianAddData(MedianFilter filter, float data)
{
  for(int i = filter.data.m-1; i > 0; i--)
    SetMatrixAt(filter.data, i, 0, GetMatrixAt(filter.data, i-1, 0));

  SetMatrixAt(filter.data, 0, 0, data);
}

// Compute the median of the given data
float MedianCompute(MedianFilter filter)
{
  Matrix sorted;
  CopyMatrixByValue(sorted, filter.data);

  // Selection Sort
  for(int i = 0; i < sorted.m; i++)
  {
    int lowestIndex = i;
    for(int j = i+1; j < sorted.m; j++)
    {
      if(GetMatrixAt(sorted, j, 0) < GetMatrixAt(sorted, lowestIndex, 0))
          lowestIndex = j;
    }
    float tmpValue = GetMatrixAt(sorted, i, 0);
    SetMatrixAt(sorted, i, 0, GetMatrixAt(sorted, lowestIndex, 0));
    SetMatrixAt(sorted, lowestIndex, 0, tmpValue);
  }

  // Get Median
  int middle = sorted.m / 2;
  float returnValue = GetMatrixAt(sorted, middle, 0);

  // Prevent Memory Leaks
  DeleteMatrix(sorted);

  // Return the median
  return returnValue;
}
