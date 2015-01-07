// BNSMatrixScalar.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/18/2014
//
// This source file includes source code that
// implements a Matrix operations for use in
// Vex Robotics Competition.  These operations
// include:
//  - MultiplyScalarToMatrix
//  - DivideScalarToMatrix
//  - VectorMagnitude
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

#ifndef __BNS_MATRIX_H
#include "BNSMatrix.h"
#endif

#ifndef __BNS_MATRIX_SCALAR_C
#define __BNS_MATRIX_SCALAR_C

void MatrixMultiplyScalar(struct Matrix *mat, float scalar)
{
  int i, j;

  for(i = 0; i < mat->m; i++)
  {
    for(j = 0; j < mat->n; j++)
    {
      SetMatrixAt(mat, i, j, GetMatrixAt(mat, i, j) * scalar);
    }
  }
}

void MatrixDivideScalar(struct Matrix *mat, float scalar)
{
  int i, j;

  for(i = 0; i < mat->m; i++)
  {
    for(j = 0; j < mat->n; j++)
    {
      SetMatrixAt(mat, i, j, GetMatrixAt(mat, i, j) / scalar);
    }
  }
}

// Euclidean Normal
float VectorMagnitude(struct Matrix *mat)
{
  float result = 0;
  int i;

  if(mat->n == 1 && mat->m > 1)
  {
    for(i = 0; i < mat->m; i++)
    {
      result += GetMatrixAt(mat, i, 0) * GetMatrixAt(mat, i, 0);
    }
    return sqrt(result);
  }
  else if(mat->m == 1 && mat->n > 1)
  {
    for(i = 0; i < mat->m; i++)
    {
      result += GetMatrixAt(mat, 0, i) * GetMatrixAt(mat, 0, i);
    }
    return sqrt(result);
  }
  else
  {
      writeDebugStreamLine("***\nBNS MATRIX ERROR\nCannot find magnitude of a non 1xn or nx1 matrix!\n***\n");
      return -1;
  }
}

#endif
