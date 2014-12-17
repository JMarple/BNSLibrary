// BNSMatrix.h
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/11/2014
//
// This header file includes source code that
// can be used to program applications that
// require complex matrix operations.
//
// Source Code:
//   BNSMatrix.c
//
// Dependencies:
//   BNSMatrixBuilder.c
//   BNSMatrixBuilder.h
//
// Basic Usage:
//   Matrix mat1;
//   CreateNewMatrix(mat1, 3, 3);
//   SetMatrixAt(mat1, 0, 0, 3.14);
//   float det = FindMatrixDeterminant(mat1);
//   etc.

#ifndef __BNS_MATRIX_H
#define __BNS_MATRIX_H

struct Matrix
{
	// Number of rows
	int m;

	// Number of columns
	int n;

	// Number f
	bool inUse;

	// Location in our "Matrix Builder" buffer
	int bufferLocation;
};

// CreateNewMatrix(...)
// Initializes memory for the matrix
// m is the height (# of rows), n is the width (# of cols)
//  aka. we are creating an mxn matrix
void CreateZerosMatrix(Matrix mat, int m, int n);

// SetMatrixAt(...)
// Sets a specific location in a matrix
// m is the row, n is the column
void SetMatrixAt(Matrix mat, int m, int n, float value);

// GetMatrixAt(...)
// Returns the value at a certain point in the matrix
// m is the row, n is the column
float GetMatrixAt(Matrix mat, int m, int n);

// MultiplyMatrix(...)
// Multiply A*B and returns to dst.
// Note: dst MUST be a different matrix from A and B
bool MultiplyMatrix(Matrix dst, Matrix A, Matrix B);

// FindInverseMatrix(...)
// Finds A^-1 and returns it to dst
// Note: dst MUST be a different matrix from A
bool FindInverseMatrix(Matrix dst, Matrix A);

// AddMatrix(...)
// Adds A + B and returns the result to dst
// Note: dst CAN be A or B without conflict
bool AddMatrix(Matrix dst, Matrix A, Matrix B);

// SubMatrix(...)
// Subtracts A - B and returns the result to dst
// Note: dst CAN be A or B without conflict
bool SubMatrix(Matrix dst, Matrix A, Matrix B);

// FindTransposeMatrix(...)
// Returns the transpose of A to dst
// Note: This currently only works for square matricies
//   and vectors!
void FindTransposeMatrix(Matrix dst, Matrix A);

// FindMatrixDeterminant(...)
// Returns the determinant of a given nxn matrix
float FindMatrixDeterminant(Matrix A);

// FindMatrixTrace(...)
// Returns the trace of Matrix A
float FindMatrixTrace(Matrix A);

// Include Dependencies
#include "BNSMatrixBuilder.h"

// Include Source Code
#include "BNSMatrix.c"

#endif
