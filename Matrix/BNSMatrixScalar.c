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

#pragma systemFile

#ifndef __BNS_MATRIX_H
  #include "BNSMatrix.h"
#endif


void MultiplyScalarToMatrix(Matrix mat, float scalar)
{
  for(int i = 0; i < mat.m; i++)
  {
    for(int j = 0; j < mat.n; j++)
    {
      SetMatrixAt(mat, i, j, GetMatrixAt(mat, i, j) * scalar);
    }
  }
}

void DivideScalarToMatrix(Matrix mat, float scalar)
{
  for(int i = 0; i < mat.m; i++)
  {
    for(int j = 0; j < mat.n; j++)
    {
      SetMatrixAt(mat, i, j, GetMatrixAt(mat, i, j) / scalar);
    }
  }
}

// Euclidean Normal
float VectorMagnitude(Matrix mat)
{
  float result = 0;

  if(mat.n == 1 && mat.m > 1)
  {
    for(int i = 0; i < mat.m; i++)
    {
      result += GetMatrixAt(mat, i, 0) * GetMatrixAt(mat, i, 0);
    }
    return sqrt(result);
  }
  else if(mat.m == 1 && mat.n > 1)
  {
    for(int i = 0; i < mat.m; i++)
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
