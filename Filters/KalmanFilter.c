#pragma systemFile

struct KalmanFilter
{
	// Data output, n-length
  Matrix dataVector; // "x"	nx1 vector

  // Sensors measurements, m-length
  Matrix measurementVector; // "z" mx1 vector

  // Prediction
	Matrix updateMatrix; // "F" nxn matrix

	// What would we expect +the matrix to be if we
	//  got PERFECT data
	Matrix extractionMatrix; // "H" mxn matrix

	// Covariance of our data
	Matrix covarianceMatrixX; // "P" nxn matrix

	// Measurement Covariance Matrix
	Matrix covarianceMatrixZ; // "R" mxm matrix

	Matrix moveVector;
};

// Kalman filter initilizations
void KalmanInitialize(KalmanFilter kal, int variables, int measurements,
											Matrix updateMatrix,
											Matrix extractionMatrix,
											Matrix covarianceMatrixX,
											Matrix covarianceMatrixZ,
											Matrix moveVector)
{
	CreateZerosMatrix(kal.dataVector, variables, 1);
	CreateZerosMatrix(kal.measurementVector, measurements, 1);

	CopyMatrix(kal.updateMatrix, updateMatrix);
	CopyMatrix(kal.extractionMatrix, extractionMatrix);
	CopyMatrix(kal.moveVector, moveVector);
	CopyMatrix(kal.covarianceMatrixX, covarianceMatrixX);
	CopyMatrix(kal.covarianceMatrixZ, covarianceMatrixZ);
}

void KalmanPredict(KalmanFilter kal)
{
	Matrix x_next;
	Matrix P_next;
	Matrix F_trans;

	// Calculate x_next (update guassian mean)
	MultiplyMatrix(x_next, kal.updateMatrix, kal.dataVector);
	AddMatrix(x_next, x_next, kal.moveVector);

	// Calculate p_next (update guassian covariance);
	MultiplyMatrix(P_next, kal.updateMatrix, kal.covarianceMatrixX);
	FindTransposeMatrix(F_trans, kal.updateMatrix);
	MultiplyMatrix(P_next, P_next, F_trans);

	// Copy results to the kalmanfilter class
	CopyMatrixByValue(kal.dataVector, x_next);
	CopyMatrixByValue(kal.covarianceMatrixX, P_next);

	DeleteMatrix(x_next);
	DeleteMatrix(P_next);
	DeleteMatrix(F_trans);
}

void KalmanUpdate(KalmanFilter kal, Matrix meas)
{
	CopyMatrix(kal.measurementVector, meas);

	// Find the difference between the move (measurement)
	//   and what we predicted (extraction * data)
	Matrix y;
	MultiplyMatrix(y, kal.extractionMatrix, kal.dataVector);
	SubMatrix(y, kal.measurementVector, y);

	// The Covariance of the move
	Matrix S;
	Matrix extTran;

	MultiplyMatrix(S, kal.extractionMatrix, kal.covarianceMatrixX);
	FindTransposeMatrix(extTran, kal.extractionMatrix);
	MultiplyMatrix(S, S, extTran);
	AddMatrix(S, S, kal.covarianceMatrixZ);

	// Kalman Gain
	Matrix K;
	Matrix Sinv;

	FindInverseMatrix(Sinv, S);
	MultiplyMatrix(K, kal.covarianceMatrixX, extTran);
	MultiplyMatrix(K, K, Sinv);

	// Figure out mean and covariance results
	Matrix x_next;
	Matrix P_next;

	MultiplyMatrix(x_next, K, y);
	AddMatrix(x_next, kal.dataVector, x_next);

	MultiplyMatrix(P_next, kal.covarianceMatrixX, extTran);
	MultiplyMatrix(P_next, P_next, Sinv);
	MultiplyMatrix(P_next, P_next, kal.extractionMatrix);
	MultiplyMatrix(P_next, P_next, kal.covarianceMatrixX);

	SubMatrix(P_next, kal.covarianceMatrixX, P_next);

	// Copy results to the kalmanfilter class
	CopyMatrixByValue(kal.dataVector, x_next);
	CopyMatrixByValue(kal.covarianceMatrixX, P_next);

	DeleteMatrix(y);
	DeleteMatrix(S);
	DeleteMatrix(extTran);
	DeleteMatrix(K);
	DeleteMatrix(Sinv);
	DeleteMatrix(x_next);
	DeleteMatrix(P_next);
}
