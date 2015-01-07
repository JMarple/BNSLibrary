// BNSHeap.h
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/11/2014
//
// This header file includes source code for
// "dynamically" create matricies through a
// set buffer size.  The heap is implemented
// simply,
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

//
#ifndef __BNS_LIB_H
#include "../BNSLib.h"
#endif

// Global variable that matricies call to
float bnsHeap[BUFFER_SIZE];

void initMemory();
bool bnsIsFree(int loc);
int bnsMalloc(int size);
void bnsFree(int loc);
void bnsDefrag();
float bnsGetHeapElement(int element);
bool bnsSetHeapElement(int element, float value);


#endif
