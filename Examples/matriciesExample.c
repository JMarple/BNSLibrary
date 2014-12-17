// matriciesExample.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/16/2014
//
// This example file shows how to use
// the matrix functionality designed
// within the BNS Library
//
// Dependencies:
//   BNSLib.h in previous directory
#include "..\BNSLib.h"

task main()
{
  // Initiate BNS Library
  BNS();

  // Create a 3x3 matrix of zeros
  Matrix mat1;
  CreateZerosMatrix(mat1, 3, 3);

  // Create a 3x3 matrix with some data in it
  // Set location 1 down, 0 across, to be 16
  Matrix mat2;
  CreateMatrix(mat2, "1.10 3.40 0; 5 3 2; 0 1 1.234");
  SetMatrixAt(mat2, 1, 0, 16);

  // Creates a 3x3 identity matrix, then multiply it by 11
  Matrix mat3;
  CreateIdentityMatrix(mat3, 3);
  MultiplyScalarToMatrix(mat3, 11);

  // Print matricies to the debugger console
  PrintMatrix(mat1);
  PrintMatrix(mat2);
  PrintMatrix(mat3);

  // Matrix Examples:

  // Matrix determinant
  float det = FindMatrixDeterminant(mat2);
  writeDebugStreamLine("Matrix Det = %f", det);

  // Matrix Inverse
  Matrix inv;
  FindInverseMatrix(inv, mat2);
  writeDebugStream("Inverse ");
  PrintMatrix(inv);

  // Matrix Multiplication
  Matrix mult;
  MatrixMultiplication(mult, mat2, mat3);
  writeDebugStream("Multiply ");
  PrintMatrix(mult);

  // Matrix Addition
  Matrix add;
  MatrixAdd(add, mat2, mat3);
  writeDebugStream("Add ");
  PrintMatrix(add);

  // Matrix Subtraction
  Matrix sub;
  MatrixSub(sub, mat3, mat2);
  writeDebugStream("Subtract ");
  PrintMatrix(sub);
}
