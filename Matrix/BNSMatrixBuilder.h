// BNSMatrixBuilder.h
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/11/2014
//
// This header file includes source code for
// "dynamically" create matricies through a
// set buffer size
//
// Source Code:
//   BNSMatrixBuilder.c
//
// Dependencies:
//   None!
//
// Basic Usage:
//   None! Used by BNSMatrix exclusively

#ifndef __BNS_MATRIX_BUILDER_H
#define __BNS_MATRIX_BUILDER_H

// Size of matrix buffer
#define BUFFER_SIZE 1000

struct MatrixBuilder
{
	// Keeps track of where new matricies are to be inputed
	int head;

	// If true, do not add or delete from the buffer
	bool locked;

	// Matrix buffer, increase this number for additional matricies
	float buffer[BUFFER_SIZE];
};

// Global variable that matricies call to
MatrixBuilder MatBuilder;

// Include Source Code
#include "BNSMatrixBuilder.c"

#endif
