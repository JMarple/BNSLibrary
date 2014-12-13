#include "BNSLib.h"

task main()
{
	initMemory();

	float dt = 0.1;

	Matrix updateMatrix;
	CreateNewMatrix(updateMatrix, 4, 4);
  SetMatrixAt(updateMatrix, 0, 0, 1);
  SetMatrixAt(updateMatrix, 0, 1, dt);
  SetMatrixAt(updateMatrix, 1, 0, 0);
  SetMatrixAt(updateMatrix, 1, 1, 1);
  SetMatrixAt(updateMatrix, 2, 2, 1);
  SetMatrixAt(updateMatrix, 2, 3, dt);
  SetMatrixAt(updateMatrix, 3, 2, 0);
  SetMatrixAt(updateMatrix, 3, 3, 1);

  Matrix extractionMatrix;
  CreateNewMatrix(extractionMatrix, 2, 4);
  SetMatrixAt(extractionMatrix, 0, 0, 1);
  SetMatrixAt(extractionMatrix, 1, 2, 1);

  Matrix covarianceMatrixX;
  CreateNewMatrix(covarianceMatrixX, 4, 4);
  SetMatrixAt(covarianceMatrixX, 0, 0, 1000);
	SetMatrixAt(covarianceMatrixX, 1, 1, 1000);
	SetMatrixAt(covarianceMatrixX, 2, 2, 1000);
	SetMatrixAt(covarianceMatrixX, 3, 3, 1000);

	Matrix covarianceMatrixZ;
	CreateNewMatrix(covarianceMatrixZ, 2, 2);
	SetMatrixAt(covarianceMatrixZ, 0, 0, 1);
	SetMatrixAt(covarianceMatrixZ, 1, 1, 1);

	Matrix moveVector;
	CreateNewMatrix(moveVector, 4, 1);

	Matrix data;
	CreateNewMatrix(data, 2, 1);
	SetMatrixAt(data, 0, 0, 1);
	SetMatrixAt(data, 1, 0, 2);

	// Initialize Kalman Filter
	KalmanFilter filter;
	KalmanInitialize(filter, 4, 2,
									updateMatrix,
									extractionMatrix,
									covarianceMatrixX,
									covarianceMatrixZ,
									moveVector);

	for(int i = 1; i <= 20; i++)
	{
		SetMatrixAt(data, 0, 0, i);
		SetMatrixAt(data, 1, 0, i*2);
		KalmanPredict(filter);
		KalmanUpdate(filter, data);
	}

	writeDebugStreamLine("Pos1 = %f, Vel1 = %f",
							GetMatrixAt(filter.dataVector, 0, 0),
							GetMatrixAt(filter.dataVector, 1, 0));

	writeDebugStreamLine("Pos2 = %f, Vel2 = %f",
							GetMatrixAt(filter.dataVector, 2, 0),
							GetMatrixAt(filter.dataVector, 3, 0));
}
