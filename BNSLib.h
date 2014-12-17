#ifndef __BNS_LIB_H
#define __BNS_LIB_H

// Core
#include "Matrix\BNSMatrix.h"

// Features
#include "Probability\MahalanobisDistance.c"
#include "Probability\NormalDistribution.c"
#include "Filters\KalmanFilter.c"

// Anything that needs to be initiated should go here
void BNS()
{
	// Start Heap
	initMemory();

	// Start random number generation
	srand(nSysTime);
}

#endif
