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

// Bit that defines if a chunk of space is free or not
#define MEM_FREE_BIT 20

// Bit that defines if a point in memory defines a
//  chunk or not.  Don't overwite if it is!
#define MEM_PROT_BIT 21

// Global variable that matricies call to
float bnsHeap[BUFFER_SIZE];

// Include Source Code
#include "BNSMatrixBuilder.c"

#endif
