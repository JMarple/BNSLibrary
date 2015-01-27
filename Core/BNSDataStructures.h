// BNSDataStructures.h
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 1/27/2014
//
// This header file includes code for storing
// data in commonly used data structures.
// Examples:
//   DynamicArray (an array with an undefined size)
//   Stack
//   ...More to come
//
// Source Code:
//   BNSDataStructures.c
//
// Dependencies:
//   BNSHeap
//   BNSCore
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


#ifndef __BNS_DATA_STRUCTURES_H
#define __BNS_DATA_STRUCTURES_H

#define DEFAULT_DYNAMIC_ARRAY_SIZE 50

// ------------------------------------------------------------------------
//
// DynamicArray is an array with an undefined size
//  Adding to the size of the array is easy by calling
//  DynamicArrayAdd(...) which will automatically expand the array
struct DynamicArray
{
	// This is the size of the array
	int size;

	// This is how much memory is actually allocated for the array
	int maxSize;

	// This signifies if this structure is being used or not
  bool inUse;

  // This points to a point in memory that our data is being put into
	int pointer;
};

// DynamicArrayInit(...)
// Initializes the DynamicArray to default values
void DynamicArrayInit(struct DynamicArray *array);

// DynamicArrayGet(...)
// Retrives data at a certain index
float DynamicArrayGet(struct DynamicArray *array, int where);

// DynamicArrayAdd(...)
// This adds a new element to the array
bool DynamicArrayAdd(struct DynamicArray *array, float value);

// DynamicArrayClear(...)
// Clears data from array, but does not free up any memory
void DynamicArrayClear(struct DynamicArray *array);

// DynamicArrayRemoveAt(...)
// Removes data at a certain index
bool DynamicArrayRemoveAt(struct DynamicArray *array, int where);

// DynamicArraySize(...)
// Returns the size of the current aray (not how much memory is allocated)
int DynamicArraySize(struct DynamicArray *array);

// DynamicArrayDelete(...)
// Frees up memory completely, note you will need to recall
//  DynamicArrayInit(...) to reuse this array
void DynamicArrayDelete(struct DynamicArray *array);

// ------------------------------------------------------------------------
//
// A stack is a data structure that allows you to access data
//  in a first-in-last-out basis. Their is no way to acess the
//  data by index, only by push/pop (add/remove) the top-most
//  element.
struct Stack
{
	// Our array of data
	struct DynamicArray array;

	// This points to the top-most index of the stack
	// The default is -1 (no data)
	int pos;
};

// StackInit(...)
// Starts the stack with it's default values
void StackInit(struct Stack *object);

// StackPop(...)
// Removes the top most element and returns it
float StackPop(struct Stack *object);

// StackPush(...)
// Adds a new element to the top of the stack
bool StackPush(struct Stack *object, float value);

// StackPeek(...)
// Shows the top most element without removing it
float StackPeek(struct Stack *object);

// StackIsEmpty(...)
// Returns true if the stack is completely empty
bool StackIsEmpty(struct Stack *object);

// ------------------------------------------------------------------------
//
#endif
