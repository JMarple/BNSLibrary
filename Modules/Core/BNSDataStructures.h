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

#define DynamicArrayInit(a,e) _DynamicArray_Init(a, sizeof(e))
#define DynamicArrayInitDefault(a,e,s) _DynamicArray_InitDefault(a, sizeof(e), s)
#define DynamicArrayAdd(a,s) _DynamicArray_Add(a, (intptr_t*)s)
#define DynamicArraySet(a,e,s) _DynamicArray_Set(a, e,(intptr_t*)s)
#define StackInit(a, e) _Stack_Init(a, sizeof(e))
#define StackPush(a, e) _Stack_Push(a, (intptr_t*)e)

#define CircularBufferInit(a,e,s) _CircularBuffer_Init(a, sizeof(e), s)
#define CircularBufferAdd(a, s) _CircularBuffer_Add(a, (intptr_t*)s)
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

	// Size of a single element in the array
	// For instance, a struct could hold multiple elements, needing more space
	//   then a single integer.
	int elementSize;
};

// DynamicArrayInit(...)
// Initializes the DynamicArray to default size
bool _DynamicArray_Init(struct DynamicArray *array, int elementSize);

// DynamicArrayInitDefault(...)
// Initializes the DynamicArray to a set size
bool _DynamicArray_InitDefault(struct DynamicArray *array, int elementSize, int setSize);

// DynamicArrayGet(...)
// Retrives data at a certain index
intptr_t DynamicArrayGet(struct DynamicArray *array, int where);

// DynamicArrayCopy(...)
// Copies an array from source to destination by reference
bool DynamicArrayCopy(struct DynamicArray *dst, struct DynamicArray src);

// DynamicArrayCopyByValue(...)
// Copies an array from source to destination by allocating new memory
bool DynamicArrayCopyByValue(struct DynamicArray *dst, struct DynamicArray src);

// DynamicArraySet(...)
// Sets a certain point in memory to a given value
void _DynamicArray_Set(struct DynamicArray *array, int where, intptr_t* fPtr);

// DynamicArrayAddEmpty(...)
// Increments the allocated memory without setting data to the heap
bool DynamicArrayAddEmpty(struct DynamicArray *array);

// DynamicArrayAdd(...)
// This adds a new element to the array
bool _DynamicArray_Add(struct DynamicArray *array, intptr_t* fPtr);

// DynamicArrayClear(...)
// Clears data from array, but does not free up any memory
void DynamicArrayClear(struct DynamicArray *array);

// DynamicArrayRemoveAt(...)
// Removes data at a certain index
bool DynamicArrayRemoveAt(struct DynamicArray *array, int where);

// DynamicArraySize(...)
// Returns the size of the current aray (not how much memory is allocated)
int DynamicArraySize(struct DynamicArray *array);

// DynamicArrayAllocatedSize(...)
// Returns the amount of allocated memory in this array
int DynamicArrayAllocatedSize(struct DynamicArray *array);

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
bool _Stack_Init(struct Stack *object, int elementSize);

// StackPop(...)
// Removes the top most element and returns it
intptr_t StackPop(struct Stack *object);

// StackPush(...)
// Adds a new element to the top of the stack
bool _Stack_Push(struct Stack *object, intptr_t* value);

// StackPeek(...)
// Shows the top most element without removing it
intptr_t StackPeek(struct Stack *object);

// StackIsEmpty(...)
// Returns true if the stack is completely empty
bool StackIsEmpty(struct Stack *object);

// ------------------------------------------------------------------------
//
// CircularBuffer

struct CircularBuffer
{
	// Our array of data
	struct DynamicArray array;

	// Head/Tail of the buffer
	int startPos;
	int endPos;
};

// CircularBufferInit(...)
// Initilizes default values into the circular buffer
bool _CircularBuffer_Init(struct CircularBuffer* object, int objectsize, int size);

bool CircularBufferIsEmpty(struct CircularBuffer* object);
bool CircularBufferIsFull(struct CircularBuffer* object);
int CircularBufferSize(struct CircularBuffer* object);
bool _CircularBuffer_Add(struct CircularBuffer* object, intptr_t* value);
intptr_t CircularBufferGet(struct CircularBuffer* object);
intptr_t CircularBufferPeek(struct CircularBuffer* object, int where);

#endif
