// BNSHeap.h
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
//   BNSHeap.c
//
// Dependencies:
//   None!

#ifndef __BNS_HEAP_H
#define __BNS_HEAP_H

// Size of matrix buffer
#define BUFFER_SIZE 1000

// Bit that defines if a chunk of space is free or not
#define MEM_FREE_BIT 20

// Bit that defines if a point in memory defines a
//  chunk or not.  Don't overwite if it is!
// Note: This is not currently being used to due to negative numbers
#define MEM_PROT_BIT 21

// Global variable that matricies call to
float bnsHeap[BUFFER_SIZE];

// Include Source Code
#include "BNSHeap.c"

#endif
