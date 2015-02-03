// BNSMatrix.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/18/2014
//
// This source file includes source code that
// implements a Matrix operations for use in
// Vex Robotics Competition.  These operations
// include:
//  - CreateZerosMatrix
//  - CreateMatrix
//  - CreateIdentityMatrix
//  - DeleteMatrix
//  - CopyMatrix
//  - CopyMatrixByValue
//  - SetMatrixAt
//  - GetMatrixAt
//  - PrintMatrix
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

#ifndef __BNS_MATRIX_C
#define __BNS_MATRIX_C

#ifndef __BNS_MATRIX_H
#include "BNSMatrix.h"
#endif
#ifndef __BNS_HEAP_H
#include "../BNSHeap.h"
#endif
#ifndef __BNS_CORE_H
#include "../BNSCore.h"
#endif

// Obtain space for a matrix filled with all zeros
void CreateZerosMatrix(struct Matrix *mat, int m, int n)
{
  int i, j;

  // Dimensions
  mat->m = m;
  mat->n = n;

  // Reserve memory for the array
  DynamicArrayInitDefault(&mat->array, m*n);

  // Initalize to all zeros
  for(i = 0; i < m; i++)
    for(j = 0; j < n; j++)
      SetMatrixAt(mat, i, j, 0);
}

// Obtain space for an identity matrix of size n
void CreateIdentityMatrix(struct Matrix* mat, int n)
{
  int i, j;

  // Dimensions
  mat->m = n;
  mat->n = n;

  // Reserve memory for the array
  DynamicArrayInitDefault(&mat->array, n*n);

  // Initialize with 1's on the main diagonal
  for(i = 0; i < n; i++)
  {
    for(j = 0; j < n; j++)
    {
      if(i==j)
        SetMatrixAt(mat, i, j, 1);
      else
        SetMatrixAt(mat, i, j, 0);
    }
  }
}

void CreateBlankMatrix(struct Matrix* mat)
{
	mat->array.inUse = false;
}

// This function takes in a string input and determines
//   how many rows and columns the user wants to have
//   ';' means a new row, spaces means new column
bool ParseMatrixString(struct Matrix* mat, char* s)
{
  int i, rows, cols;
  bool numTrig;
  char lastRealChar;

  // Index/counters
  i = 0;
  rows = 1;
  cols = 0;

  // This lets us trigger between numbers and non-numbers
  // When this is true, we are looking at a number,
  //   but when it switchs from true to false, we can
  //   increment the col variable to count the amount of nums
  numTrig = false;
  lastRealChar = 0;// A crap hack, ignore final ';' if accidently inputted.

  // Determine number of rows and columns
  while(s[i] != 0)
  {
    // Semicolons determine new rows
    if(s[i] == ';')
    {
      lastRealChar = ';';
      rows++;
      if(numTrig == true) cols++;

      numTrig = false;
    }
    // If a space, we know it's not a number
    else if(s[i] == ' ')
    {
      if(numTrig == true)	cols++;

      numTrig = false;
    }
    // Look for hidden tabs/enters being parsed
    else if(s[i] == 9 || s[i] == 10){numTrig = false;}
    // Must be a number, so set numTrig true so we can watch
    //  for a non-number
    else
    {
      lastRealChar = ' ';
      numTrig = true;
    }

    // Increment string index
    i++;
  }
  if(numTrig == true)
    cols++;

  // Bleh
  if(lastRealChar == ';')
    rows--;

  if(cols%rows != 0)
  {
    //writeDebugStreamLine("%d %d", cols, rows);
    BNS_ERROR("MATRIX ERROR", "Cannot parse matrix string in CreateMatrix(...), inconsistent rows or columns!");
    return false;
  }
  cols /= (int)rows;

  // Set Parameters
  mat->m = rows;
  mat->n = cols;

   // Reserve memory for the array
  DynamicArrayInitDefault(&mat->array, rows*cols);

  return true;
}

// Obtain space for a matrix given a string as an input
bool CreateMatrix(struct Matrix* mat, char* s)
{
  char* cpy = s;
  int i, j, k, tmpIndex;
  bool numTrig;

  // Parse the given string and save the results to mat
  if(!ParseMatrixString(mat, s))
    return false;

  int sIndex = 0;

  // Fill empty matrix with the information
  for(i = 0; i < mat->m; i++)
  {
    for(j = 0; j < mat->n; j++)
    {
      char tmp[16];
      for(k = 0; k < 16; k++)
        tmp[k] = 0;

      tmpIndex = 0;
      numTrig = false;

      while(true)
      {
        if(s[sIndex] >= 45 && s[sIndex] <= 60)
        {
          tmp[tmpIndex] = s[sIndex];
          tmpIndex++;
          numTrig = true;
        }
        else if(numTrig == true)
          break;

        sIndex++;
      }

      SetMatrixAt(mat, i, j, atof(tmp));
    }
  }

  return true;
}

// Free space given a matrix
void DeleteMatrix(struct Matrix *mat)
{
  mat->m = 0;
  mat->n = 0;
 	DynamicArrayDelete(&mat->array);
}

// Get a copy of a matrix by allocating new space and
//   directly copying all values to the new matrix
bool CopyMatrixByValue(struct Matrix* dst, struct Matrix src)
{
  bool result = DynamicArrayCopyByValue(&dst->array, src.array);
  if(result == true)
  {
  	dst->m = src.m;
  	dst->n = src.n;
  }

  return result;
}

// Get a copy of a matrix by getting a reference to
//  the original data
bool CopyMatrix(struct Matrix *dst, struct Matrix src)
{
  dst->m = src.m;
  dst->n = src.n;
  DynamicArrayCopy(&dst->array, src.array);

  return true;
}

// Set an element in a matrix, given m-down and n-across
void SetMatrixAt(struct Matrix *mat, int m, int n, float value)
{
	if(m <= mat->m && n <= mat->n)
  {
  	DynamicArraySet(&mat->array, mat->n * m + n, value);
  }
}

// Get an element in a matrix given m-down and n-across
float GetMatrixAt(struct Matrix *mat, int m, int n)
{
	return DynamicArrayGet(mat->array, mat->n * m + n);
}

// Print a Matrix to the console
void PrintMatrix(struct Matrix *A)
{
  int i, j;

  if(A->array.inUse == true)
  {
    writeDebugStream("Matrix: %d\n", A->array.pointer);
    for(i = 0; i < A->m; i++)
    {
      for(j = 0; j < A->n; j++)
      {
        writeDebugStream("%f\t", GetMatrixAt(A, i, j));
      }
      writeDebugStream("\n");
    }
  }
  else
  {
    BNS_ERROR("MATRIX ERROR", "Cannot Print an unset or NULL matrix!");
  }
}

#endif
