#include "BNSLib.h"

task main()
{
  Matrix data;
  CreateNewMatrix(data, 3, 1);
  SetMatrixAt(data, 0, 0, 1);
  SetMatrixAt(data, 1, 0, 10);
  SetMatrixAt(data, 2, 0, 0);

  Matrix mean;
  CreateNewMatrix(mean, 3, 1);
  SetMatrixAt(mean, 0, 0, 1);
  SetMatrixAt(mean, 1, 0, 10);
  SetMatrixAt(mean, 2, 0, 0);

  Matrix cov;
  CreateNewMatrix(cov, 3, 3);
  SetMatrixAt(cov, 0, 0, 0.1);
  SetMatrixAt(cov, 1, 1, 0.1);
  SetMatrixAt(cov, 2, 2, 0.1);

  //float result = 0;
  float result = MultivariateNormalDistribution(data, mean, cov);
  writeDebugStreamLine("Dist = %f", result);
}
