#include "BNSLib.h"

task main()
{
	BNS();

	float dt = 0.1;

	Matrix updateMatrix;
	CreateZerosMatrix(updateMatrix, 4, 4);
	SetMatrixAt(updateMatrix, 0, 0, 1);
	SetMatrixAt(updateMatrix, 0, 1, dt);
	SetMatrixAt(updateMatrix, 1, 0, 0);
	SetMatrixAt(updateMatrix, 1, 1, 1);
	SetMatrixAt(updateMatrix, 2, 2, 1);
	SetMatrixAt(updateMatrix, 2, 3, dt);
	SetMatrixAt(updateMatrix, 3, 2, 0);
	SetMatrixAt(updateMatrix, 3, 3, 1);

	Matrix extractionMatrix;
	CreateMatrix(extractionMatrix,
	"1 0 0 0;
	0 0 1 0");

	Matrix covarianceMatrixX;
	CreateMatrix(covarianceMatrixX,
	"1000 0 0 0;
	0 1000 0 0;
	0 0 1000 0;
	0 0 0 1000");

	Matrix covarianceMatrixZ;
	CreateMatrix(covarianceMatrixZ, "1 0; 0 1");

	Matrix moveVector;
	CreateZerosMatrix(moveVector, 4, 1);

	Matrix data;
	CreateZerosMatrix(data, 2, 1);
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
