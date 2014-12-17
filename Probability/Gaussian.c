#pragma systemFile

// Function Prototype
float MultivariateNormalDistribution(Matrix data, Matrix mean, Matrix cov);

// Basic Gaussian Structure
struct Gaussian
{
	Matrix mean;
	Matrix cov;
};

// Initialize basic values for a gaussian
void GaussianInit(Gaussian gau, Matrix mean, Matrix cov)
{
  DeleteMatrix(gau.mean);
	DeleteMatrix(gau.cov);
	CopyMatrixByValue(gau.mean, mean);
	CopyMatrixByValue(gau.cov, cov);
}

// Calculate the probabilty distribution function
float GaussianPDF(Gaussian gau, Matrix data)
{
	return MultivariateNormalDistribution(data, gau.mean, gau.cov);
}

// Mahalanobis Distance, good for finding distances within a guassian
float FindSquareMahalanobisDistance(Matrix data, Matrix mean, Matrix cov)
{
	Matrix sub1;
	Matrix sub2;
	Matrix result;
	Matrix inverse;
	CreateZerosMatrix(sub1, data.m, 1);
	CreateZerosMatrix(sub2, 1, data.m);
	CreateZerosMatrix(inverse, cov.m, cov.n);
	CreateZerosMatrix(result, 1, 1);

	// (x-u)^T*cov^-1*(x-u)

	// Subtract the data from the mean to find an error
  MatrixSub(sub1, data, mean);

  // Tranpose vector
  FindTransposeMatrix(sub1, sub1);

  // Find inverse, it it doesn't exist, there is an issue!
  if( !FindInverseMatrix(inverse, cov) )
  	return 0;

  // Multiply the Tranposed "error" and the inverse of covarience
  MatrixMultiplication(sub2, sub1, inverse);

  // Go back to original vector
  FindTransposeMatrix(sub1, sub1);

  MatrixMultiplication(result, sub2, sub1);

  float finalResult = GetMatrixAt(result, 0, 0);

  DeleteMatrix(sub1);
  DeleteMatrix(sub2);
  DeleteMatrix(inverse);
  DeleteMatrix(result);

  return finalResult;
}

float FindMahalanobisDistance(Matrix data, Matrix mean, Matrix cov)
{
	return sqrt(FindSquareMahalanobisDistance(data, mean, cov));
}

// Multivariate Normal Guassian Distribution
//  Returns the PDF of a set of data, given a guassian mean and covariance
//  See the example for more details.
float MultivariateNormalDistribution(Matrix data, Matrix mean, Matrix cov)
{
	float mahDist = -0.5 * FindSquareMahalanobisDistance(data, mean, cov);

	float normalizer = sqrt( pow(6.28318, data.m) * FindMatrixDeterminant(cov) );

	return 1 / normalizer * exp(mahDist);
}
