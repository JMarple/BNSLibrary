#pragma systemFile

float FindSquareMahalanobisDistance(Matrix data, Matrix mean, Matrix cov)
{
	Matrix sub1;
	Matrix sub2;
	Matrix result;
	Matrix inverse;
	CreateNewMatrix(sub1, data.m, 1);
	CreateNewMatrix(sub2, 1, data.m);
	CreateNewMatrix(inverse, cov.m, cov.n);
	CreateNewMatrix(result, 1, 1);

	// (x-u)^T*cov^-1*(x-u)

	// Subtract the data from the mean to find an error
  SubMatrix(sub1, data, mean);

  // Tranpose vector
  FindTransposeMatrix(sub1, sub1);

  // Find inverse, it it doesn't exist, there is an issue!
  if( !FindInverseMatrix(inverse, cov) )
  	return 0;

  // Multiply the Tranposed "error" and the inverse of covarience
  MultiplyMatrix(sub2, sub1, inverse);

  // Go back to original vector
  FindTransposeMatrix(sub1, sub1);


  MultiplyMatrix(result, sub2, sub1);

  // NOT THREAD SAFE!!!
  popBufferLocation(data.m);
  popBufferLocation(data.m);
  popBufferLocation(1);

  return GetMatrixAt(result, 0, 0);
}

float FindMahalanobisDistance(Matrix data, Matrix mean, Matrix cov)
{
	return sqrt(FindSquareMahalanobisDistance(data, mean, cov));
}
