#include "Matrix\BNSMatrix.h"

task main()
{
  Matrix mat1;
  CreateNewMatrix(mat1, 3, 2);
  SetMatrixAt(mat1, 0, 0, 1);
  SetMatrixAt(mat1, 1, 0, 3);
  SetMatrixAt(mat1, 2, 0, 5);
  SetMatrixAt(mat1, 0, 1, 2);
  SetMatrixAt(mat1, 1, 1, 4);
  SetMatrixAt(mat1, 2, 1, 6);

  Matrix mat2;
  CreateNewMatrix(mat2, 2, 4);
  SetMatrixAt(mat2, 0, 0, 1);
  SetMatrixAt(mat2, 0, 1, 2);
  SetMatrixAt(mat2, 0, 2, 3);
  SetMatrixAt(mat2, 0, 3, 4);
  SetMatrixAt(mat2, 1, 0, 5);
  SetMatrixAt(mat2, 1, 1, 6);
  SetMatrixAt(mat2, 1, 2, 7);
  SetMatrixAt(mat2, 1, 3, 8);

  Matrix result;
  CreateNewMatrix(result, 3, 4);

  MultiplyMatrix(result, mat1, mat2);

  PrintMatrix(mat1);
  PrintMatrix(mat2);
  PrintMatrix(result);

}
