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

#include "..\BNSLib.h"

task main()
{
  // Initiate BNS Library
  BNS();

  // Create a 3x3 matrix of zeros
  Matrix mat1;
  CreateZerosMatrix(&mat1, 3, 3);

  // Create a 3x3 matrix with some data in it
  // Set location 1 down, 0 across, to be 16
  Matrix mat2;
  CreateMatrix(&mat2, "1.10 3.40 0; 5 3 2; 0 1 1.234");
  SetMatrixAt(&mat2, 1, 0, 16);

  // Creates a 3x3 identity matrix, then multiply it by 11
  Matrix mat3;
  CreateIdentityMatrix(&mat3, 3);
  MatrixMultiplyScalar(&mat3, 11);

  // Print matricies to the debugger console
  PrintMatrix(&mat1);
  PrintMatrix(&mat2);
  PrintMatrix(&mat3);

  // Matrix Examples:

  // Matrix determinant
  float det = MatrixDeterminant(&mat2);
  writeDebugStreamLine("Matrix Det = %f", det);

  // Matrix Inverse
  Matrix inv;
  MatrixInv(&inv, mat2);
  writeDebugStream("Inverse ");
  PrintMatrix(&inv);

  // Matrix Multiplication
  Matrix mult;
  MatrixMult(&mult, mat2, mat3);
  writeDebugStream("Multiply ");
  PrintMatrix(mult);

  // Matrix Addition
  Matrix add;
  MatrixAdd(&add, mat2, mat3);
  writeDebugStream("Add ");
  PrintMatrix(&add);

  // Matrix Subtraction
  Matrix sub;
  MatrixSub(&sub, mat3, mat2);
  writeDebugStream("Subtract ");
  PrintMatrix(&sub);
}
