// BNSMatrixArithmetic.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/18/2014
//
// This source file includes source code that
// implements a Matrix operations for use in
// Vex Robotics Competition.  These operations
// include:
//  - MatrixAdd
//  - MatrisSub
//  - MatrixMult
//  - MatrixInv
//  - MatrixDeterminant
//  - MatrixTranspose
//  - MatrixTrace
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

#ifndef __BNS_MATRIX_ARITH_C
#define __BNS_MATRIX_ARITH_C

// Multiply Matricies as AB, saving to dst
bool MatrixMult(struct Matrix *dst, struct Matrix A, struct Matrix B)
{
  struct Matrix tmpCol, dstCopy;
  int row, col, k;
  float sum;

  if(A.n != B.m)
  {
    writeDebugStreamLine("***\nBNS MATRIX ERROR\nCannot Multiply a %d, %d to a %d, %d matrix, incorrect sizes\n***\n", A.m, A.n, B.m, B.n);
    return false;
  }

  CreateZerosMatrix(&tmpCol, B.m, 1);
  CreateZerosMatrix(&dstCopy, A.m, B.n);

  // Loop through each row and column of our second matrix
  for(row = 0; row < A.m; row++)
  {
    for(col = 0; col < B.n; col++)
    {
      sum = 0;

      for(k = 0; k < B.m; k++)
      {
        sum += GetMatrixAt(&A, row, k) * GetMatrixAt(&B, k, col);
      }
      SetMatrixAt(&dstCopy, row, col, sum);
    }
  }

  // Copy our results matrix to our destination matrix
  DeleteMatrix(dst);
  CreateZerosMatrix(dst, dstCopy.m, dstCopy.n);
  CopyMatrixByValue(dst, dstCopy);

  DeleteMatrix(&tmpCol);
  DeleteMatrix(&dstCopy);

  return true;
}

void SwapRowsInMatrix(struct Matrix *A, int row1, int row2)
{
  struct Matrix tmpRow;
  int i;
  CreateZerosMatrix(&tmpRow, 1, A->n);

  // Copy first row
  for(i = 0; i < A->n; i++)
    SetMatrixAt(&tmpRow, 0, i, GetMatrixAt(A, row1, i));

  // Copy second row to first row
  for(i = 0; i < A->n; i++)
    SetMatrixAt(A, row1, i, GetMatrixAt(A, row2, i));

  // Copy tmpRow to secondRow
  for(i = 0; i < A->n; i++)
    SetMatrixAt(A, row2, i, GetMatrixAt(&tmpRow, 0, i));

  // Free up the space we took to write the tmpRow
  DeleteMatrix(&tmpRow);
}

// Searches through all the rows in a single column to find the largest number in it
int GetLargestColumnFromMatrix(struct Matrix *A, int col, int row1, int row2)
{
  int largest, index, val, i;
  largest = GetMatrixAt(A, row1, col);
  index = row1;

  for(i = row1+1; i < row2; i++)
  {
    val = GetMatrixAt(A, row1, i);
    if(val > largest)
    {
      largest = val;
      index = i;
    }
  }

  return index;
}

// Add Matricies together A+B
bool MatrixAdd(struct Matrix *dst, struct Matrix A, struct Matrix B)
{
  int i, j;

  if(A.m != B.m || A.n != B.n)
  {
    writeDebugStreamLine("***\nBNS MATRIX ERROR\nCannot Add a %d, %d to a %d, %d matrix, not the same size\n***\n", A.m, A.n, B.m, B.n);
    return false;
  }
  if(dst->array.inUse == false || dst->m != A.m || dst->n != A.n)
  {
    DeleteMatrix(dst);
    CreateZerosMatrix(dst, A.m, A.n);
  }

  for(i = 0; i < A.m; i++)
  {
    for(j = 0; j < A.n; j++)
    {
      SetMatrixAt(dst, i, j,
      GetMatrixAt(&A, i, j) + GetMatrixAt(&B, i, j)
      );
    }
  }

  return true;
}

// Subtract Matricies together A-B
bool MatrixSub(struct Matrix *dst, struct Matrix A, struct Matrix B)
{
  int i,j;
  if(A.m != B.m || A.n != B.n)
  {
    writeDebugStreamLine("***\nBNS MATRIX ERROR\nCannot Subtract a %d, %d to a %d, %d matrix, not the same size\n***\n", A.m, A.n, B.m, B.n);
    return false;
  }
  if(dst->array.inUse == false || dst->m != A.m || dst->n != A.n)
  {
    DeleteMatrix(dst);
    CreateZerosMatrix(dst, A.m, A.n);
  }
  for(i = 0; i < A.m; i++)
  {
    for(j = 0; j < A.n; j++)
    {
      SetMatrixAt(dst, i, j,
      GetMatrixAt(&A, i, j) - GetMatrixAt(&B, i, j)
      );
    }
  }

  return true;
}

// Returns the determinant of Matrix A
float MatrixDeterminant(struct Matrix *A)
{
  float sumOfDet;
  struct Matrix detMat;
  int multiplier, col, row, index, i;

  // Ensure this is a square matrix
  if(A->m != A->n)
  {
    writeDebugStreamLine("***\nBNS MATRIX ERROR\nCannot find a determinant of a %d by %d matrix, not a square\n***\n", A->m, A->n);
    return 0;
  }
  if(A->m == 1)
  {
    return GetMatrixAt(A, 0, 0);
  }
  else if(A->m == 2)
  {
    return GetMatrixAt(A, 0, 0) * GetMatrixAt(A, 1, 1) - GetMatrixAt(A, 0, 1) * GetMatrixAt(A, 1, 0);
  }
  else
  {
    sumOfDet = 0;
    CreateZerosMatrix(&detMat, A->m-1, A->n-1);

    multiplier = 1;
    for(col = 0; col < A->n; col++)
    {
      index = 0;
      for(i = 0; i < A->n; i++)
      {
        // If we aren't skipping this column
        if(col != i)
        {
          for(row = 1; row < A->m; row++)
          {
            SetMatrixAt(&detMat, row-1, index, GetMatrixAt(A, row, i));
          }
          index++;
        }
      }

      sumOfDet += multiplier * GetMatrixAt(A, 0, col) * MatrixDeterminant(&detMat);
      multiplier *= -1;
    }

    DeleteMatrix(&detMat);

    return sumOfDet;
  }

  return 0;
}

// Returns the sum of the main diagonal
float MatrixTrace(struct Matrix *A)
{
  float trace;
  int i;

  // Ensure this is a square matrix
  if(A->m != A->n)
  {
    writeDebugStreamLine("***\nBNS MATRIX ERROR\nCannot find the trace of a %d by %d matrix, not a square matrix!\n***\n", A->m, A->n);
    return 0;
  }

  // Return Trace
  trace = 1;
  for(i = 0; i < A->n; i++)
  {
    trace += GetMatrixAt(A, i, i);
  }

  return trace;
}

// Returns a new matrix of minors
bool FindMatrixOfMinors(struct Matrix *dst, struct Matrix A)
{
  struct Matrix tmp;
  int i, j, x, m, n;
  float det;

  // Ensure this is a square matrix
  if(A.m != A.n)
    return false;

  CreateZerosMatrix(&tmp, A.m-1, A.m-1);

  // Go through each i, j to find matrix of minors
  for(i = 0; i < A.m; i++)
  {
    for(j = 0; j < A.n; j++)
    {
      x = 0;

      // Go through each m, n to find determinant
      for(m = 0; m < A.m-1; m++)
      {
        int y = 0;
        for(n = 0; n < A.n-1; n++)
        {
          SetMatrixAt(&tmp, y, x, GetMatrixAt(&A, n+(n>=j), m+(m>=i)));
          y++;
        }
        x++;
      }
      det = MatrixDeterminant(&tmp);
      SetMatrixAt(dst, j, i, det);
    }
  }

  DeleteMatrix(&tmp);

  return true;
}

bool FindCofactorMatrix(struct Matrix *dst, struct Matrix A)
{
  int i, j;

  // Ensure this is a square matrix
  if(A.m != A.n)
    return false;

  FindMatrixOfMinors(dst, A);

  for(i = 0; i < dst->m; i++)
  {
    for(j = 0; j < dst->m; j++)
    {
      if((i+j)%2 == 1)
      {
        SetMatrixAt(dst, i, j, GetMatrixAt(dst, i, j) * -1);
      }
    }
  }

  return true;
}

// Only works for square matricies and vectors atm
void MatrixTranspose(struct Matrix *dst, struct Matrix A)
{
  struct Matrix tmpDst;
  int i, j;

  CreateZerosMatrix(&tmpDst, A.n, A.m);

  for(i = 0; i < A.m; i++)
  {
    for(j = 0; j < A.n; j++)
    {
      SetMatrixAt(&tmpDst, j, i, GetMatrixAt(&A, i, j));
    }
  }

  DeleteMatrix(dst);
  CreateZerosMatrix(dst, tmpDst.m, tmpDst.n);
  CopyMatrixByValue(dst, tmpDst);

  DeleteMatrix(&tmpDst);
}

bool MatrixInv(struct Matrix *dst, struct Matrix A)
{
  struct Matrix dstTmp;
  bool realResult;
  float det;
  int i, j;

  // Ensure this is a square matrix
  if(A.m != A.n)
    return false;

  // If a 1x1 Matrix, just return the one value ^ -1
  if(A.m == 1)
  {
  	CreateZerosMatrix(dst, 1, 1);
  	SetMatrixAt(dst, 0, 0, 1.0/GetMatrixAt(A, 0, 0));
  	return true;
  }

  CreateZerosMatrix(&dstTmp, A.m, A.n);

  realResult = true;

  FindCofactorMatrix(&dstTmp, A);

  MatrixTranspose(&dstTmp, dstTmp);

  det = MatrixDeterminant(&A);

  if(det == 0)
    realResult = false;

  for(i = 0; i < A.m; i++)
  {
    for(j = 0; j < A.n; j++)
    {
      SetMatrixAt(&dstTmp, i, j, GetMatrixAt(&dstTmp, i, j) / det);
    }
  }

  DeleteMatrix(dst);
  CreateZerosMatrix(dst, A.m, A.n);
  CopyMatrixByValue(dst, dstTmp);
  DeleteMatrix(&dstTmp);

  return realResult;
}

#endif
