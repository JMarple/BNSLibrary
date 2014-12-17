// normalDistributionExample.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/16/2014
//
// This example file shows how to use
// the multivariate PDF included in the
// BNSLibrary
//
// Dependencies:
//   BNSLib.h in previous directory
#include "..\BNSLib.h"

task main()
{
	// Initiate BNS Library
	BNS();

	// Our input data
	Matrix data;
	CreateMatrix(data, "1.1; 10.1; 0");

	// Guassian Mean
	// Ie. Where is the guassisan located?
	Matrix mean;
	CreateMatrix(mean, "1; 10; 0");

	// Guassian covariance
	// How "good" is our mean?
	// A low number like 0.1 means we're very certain
	// A high number like 1000 means we have little knowledge
	//  of the "true" mean
	Matrix cov;
	CreateMatrix(cov, "0.1 0 0; 0 0.1 0; 0 0 0.1;");

	// Print Result of the PDF
	float result = MultivariateNormalDistribution(data, mean, cov);
	writeDebugStreamLine("Dist = %f", result);

}
