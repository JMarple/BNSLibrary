float MultivariateNormalDistribution(Matrix data, Matrix mean, Matrix cov)
{
	float mahDist = -0.5 * FindSquareMahalanobisDistance(data, mean, cov);

	float normalizer = sqrt( pow(6.28318, data.m) * FindMatrixDeterminant(cov) );

	return 1 / normalizer * exp(mahDist);
}
