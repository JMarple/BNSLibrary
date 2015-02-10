// BNSHeap.h
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/11/2014
//
// This header file includes source code for
// "dynamically" create matricies through a
// set buffer size.
//
// Source Code:
//   BNSHeap.c
//
// Dependencies:
//   None!
//
// ------------------------------------------------------------------------
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// The author can be contacted by email at jmarple@umass.edu
//
// ------------------------------------------------------------------------

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

// Some unlikely number that can be returned as an error code
// Binary equivilant is:
//   1010 1010 0000 0000 0000 0000 0000 0000
// So hopefully if a user tries to set this as a
// a motor value, it will be cropped to zero
#define BNS_ERROR_CODE 2852126720

#ifndef __BNS_LIB_H
#include "../BNSLib.h"
#endif

// Global variable that matricies call to
float bnsHeap[BUFFER_SIZE];

// initMemory(...)
// Set Default values for the heap
void initMemory();

// bnsIsFree(...)
// Returns true/false if a location contains
//  free space or not
bool bnsIsFree(int loc);

// bnsMalloc(...)
// This allocates space in the heap for use
int bnsMalloc(int size);

// bnsFree(...)
// This frees memory that was already taken
void bnsFree(int loc);

// bnsDefrag(...)
// This combines consecutive chunks of free memory
void bnsDefrag();

// bnsGetHeapElement(...)
// This returns the element at a memory location
float bnsGetHeapElement(int element);

// bnsSetHeapElement(...)
// This sets the element at a memory location
// Note: this is prone to heap corruption if you
//  write to an incorrect memory location!
bool bnsSetHeapElement(int element, float value);

// bnsPrintMemory(...)
// Prints the memory contents from memory location start to end
// "*" designates free memory, "|" designates protected memory
void bnsPrintMemory(int startPos, int endPos);

// bnsPrintMemoryDetails(...)
// Prints details about memory usage
void bnsPrintMemoryDetails();

#endif
