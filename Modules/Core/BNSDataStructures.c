// BNSDatastructures.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 1/27/2014
//
// This source file includes source code that
// implements commonly used data structures
// This includes:
//   DynamicArray
//   Stack
//   CircularBuffer
//
// Dependencies:
//    BNSDataStructures.h
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

#pragma systemFile

#ifndef __BNS_DATA_STRUCTURES_C
#define __BNS_DATA_STRUCTURES_C

#ifndef __BNS_DATA_STRUCTURES_H
#include "BNSDataStructures.h"
#endif

#ifndef __BNS_HEAP_H
#include "BNSHeap.h"
#endif

#ifndef __BNS_CORE_H
#include "BNSCore.h"
#endif

// Initializes a dynamic array in memory
// Default size is in BNSDataStructures.h as
//  DEFAULT_DYNAMIC_ARRAY_SIZE
bool _DynamicArray_Init(struct DynamicArray *array, int elementSize)
{
  DynamicArrayClear(array);

  array->pointer = bnsMalloc(DEFAULT_DYNAMIC_ARRAY_SIZE);

  if(array->pointer != -1)
  {
    // Set element size to number of words (32bits) needed in the heap
    //  to sore this datatype.
    // For instance,
    //  if 5 bytes are needed, (5-4)/(4) + 1 = 2 words to store 5 bytes
    //  if 4 bytes are needed, (3-4)/(4) + 1 = 1 word to store 4 bytes
    array->elementSize = (elementSize-1)/(sizeof(int32_t)) + 1;

    array->maxSize = DEFAULT_DYNAMIC_ARRAY_SIZE;
    array->size = 0;
    array->inUse = true;

    return true;
  }
  else
  {
  	return false;
  }
}

// Initialize a dynamic array to a default size
// SetSize is how much memory will be allocated by default
bool _DynamicArray_InitDefault(struct DynamicArray *array, int elementSize, int setSize)
{
	DynamicArrayClear(array);

	// Set element size to number of words (32bits) needed in the heap
  //  to sore this datatype.
  // For instance,
  //  if 5 bytes are needed, (5-4)/(4) + 1 = 2 words to store 5 bytes
  //  if 4 bytes are needed, (3-4)/(4) + 1 = 1 word to store 4 bytes
  array->elementSize = (elementSize-1)/(sizeof(int32_t)) + 1;

	array->pointer = bnsMalloc(setSize*array->elementSize);

	if(array->pointer != -1)
	{
		array->maxSize = setSize;
		array->size = 0;
		array->inUse = true;
		return true;
	}
	else
	{
		return false;
	}
}

// Automatically adds an empty element into DynamicArray
// If new space is needed, it will automatically allocate that
//   memory
bool DynamicArrayAddEmpty(struct DynamicArray *array)
{
	if(array->size*array->elementSize < array->maxSize)
  {
    array->size+=array->elementSize;
  }
  else
  {
    //writeDebugStreamLine("%d Trying to extend", array->size);
    int res = bnsExtend(array->pointer, array->maxSize*2);
    if(res != -1)
    {
      array->pointer = res;
      array->maxSize *= 2;
      array->size+=array->elementSize;
    }
    else
    {
      BNS_ERROR("DYNAMIC ARRAY", "Ran out of memory! Check DynamicArrayAddEmpty(...)");
      return false;
    }
  }

  return true;
}

// Automatically adds an element into DynamicArray
// If new space is needed, it will automatically allocate that
//   memory
// Note whatever is located at fPtr will be copied into the heap,
//   so the original variable can be reused if wanted
bool _DynamicArray_Add(struct DynamicArray *array, intptr_t* fPtr)
{
  if(array->size*array->elementSize < array->maxSize)
  {
  	for(int i = 0; i < array->elementSize; i++)
    	bnsSetHeapElement(array->pointer + array->size*array->elementSize + i, (int32_t)*(fPtr+i));

    array->size+=array->elementSize;
  }
  else
  {
    //writeDebugStreamLine("%d Trying to extend", array->size);
    int res = bnsExtend(array->pointer, array->maxSize*2);
    if(res != -1)
    {
      array->pointer = res;
      array->maxSize *= 2;
      for(int i = 0; i < array->elementSize; i++)
      	bnsSetHeapElement(array->pointer + array->size*array->elementSize + i, (int32_t)*(fPtr+i));

      array->size+=array->elementSize;
    }
    else
    {
      BNS_ERROR("DYNAMIC ARRAY", "Ran out of memory! Check DynamicArrayAdd(...)");
      return false;
    }
  }

  return true;
}

// Adds a number to the dynamic array
void DynamicArrayAddInteger(struct DynamicArray *array, int32_t value)
{
	int32_t tmpV = value;
	DynamicArrayAdd(array, &tmpV);
}

// Sets a point in memory
void _DynamicArray_Set(struct DynamicArray *array, int where, intptr_t* fPtr)
{
	if(where*array->elementSize < array->maxSize)
	{
		// This makes sure the pointer to where to add data isn't
	  //  overwriting data
		if(where*array->elementSize >= array->size)
			array->size = where*array->elementSize+1;

		for(int i = 0; i < array->elementSize; i++)
    	bnsSetHeapElement(array->pointer + where*array->elementSize + i, (int32_t)*(fPtr+i));
	}
	else
	{
		// TODO: Need to expand the memory?
	}
}

// Copies an array from source to destination by reference,
//  note allocating new memory
bool DynamicArrayCopy(struct DynamicArray *dst, struct DynamicArray src)
{
	dst->maxSize = src.maxSize;
	dst->size = src.size;
	dst->inUse = src.inUse;
	dst->pointer = src.pointer;
	dst->elementSize = src.elementSize;

	return true;
}

// Copy a matrix by allocating new memory for it
bool DynamicArrayCopyByValue(struct DynamicArray *dst, struct DynamicArray src)
{
	int i;
	DynamicArrayDelete(dst);
	DynamicArrayInitDefault(dst, float, src.maxSize);

	dst->inUse = true;
	for(i = 0; i < dst->maxSize; i++)
	{
		DynamicArraySet(dst, i, DynamicArrayGet(&src, i));
	}

	dst->size = src.size;
	return true;
}

// Retrivies a point of memory from the array
intptr_t DynamicArrayGet(struct DynamicArray *array, int where)
{
	// Throw error if trying to use unallocated memory
	if(where*array->elementSize >= array->size)
		BNS_ERROR("DYNAMIC ARRAY", "TRYING TO ALLOCATE MEMORY OUTSIDE DYNAMIC ARRAY SIZE");

	int thePointer = array->pointer;
	int theSize = array->elementSize;

	intptr_t* x = bnsGetHeapElementMemory(array->pointer + where*array->elementSize);
	return x;
}

// Removes a certain element from the array
bool DynamicArrayRemoveAt(struct DynamicArray *array, int where)
{
  if(where < array->maxSize && where >= 0)
  {
    for(int i = where; i < array->maxSize-1; i++)
    {
      bnsSetHeapElement(array->pointer + i, bnsGetHeapElement(array->pointer + i+1));
    }
    bnsSetHeapElement(array->pointer + array->maxSize-1, 0);
    array->size--;
  }
  else
  {
    BNS_ERROR("DYNAMIC ARRAY", "Trying to remove unallocated memory, check calls to DynamicArrayRemoveAt(...)");
    return false;
  }

  return true;
}

// Clears the entire array to a size of zero
// Note: Memory is still allocated!
void DynamicArrayClear(struct DynamicArray *array)
{
  array->size = 0;
}

// Returns how large the array is
int DynamicArraySize(struct DynamicArray *array)
{
  return array->size/array->elementSize;
}

// Returns how much memory has been allocated
int DynamicArrayAllocatedSize(struct DynamicArray *array)
{
	return array->maxSize;
}

// Removes the array from memory entirely
// Note: You will have to call DynamicArrayInit to
//  be able to use this array again
void DynamicArrayDelete(struct DynamicArray *array)
{
  array->size = 0;
  if(array->inUse == true)
  {
    bnsFree(array->pointer);
    array->pointer = -1;
  }
}

// Initialize the stack
bool _Stack_Init(struct Stack *object, int elementSize)
{
  object->pos = -1;
  return _DynamicArray_Init(object->array, elementSize);
}

// Removes the top-most number and returns it
//  to the user
intptr_t StackPop(struct Stack *object)
{
  if(object->pos < 0)
  {
    BNS_ERROR("STACK ERROR", "Cannot pop a stack of size zero!");
    return 0;
  }

  intptr_t returnVal = DynamicArrayGet(object->array, object->pos);
  object->pos--;

  return returnVal;
}

// Puts an element to the top of the stack
bool _Stack_Push(struct Stack *object, intptr_t* value)
{
  bool good = DynamicArrayAdd(object->array, value);

  if(good)
  {
    object->pos++;
    return true;
  }
  else
  {
    return false;
  }
}

// Look at the top element without removing it
intptr_t StackPeek(struct Stack *object)
{
  if(object->pos < 0)
  {
    BNS_ERROR("STACK ERROR", "Cannot peek a stack of size zero!");
    return 0;
  }

  return DynamicArrayGet(object->array, object->pos);
}

// Returns true if the stack is completely empty
bool StackIsEmpty(struct Stack *object)
{
  if(object->pos < 0)
    return true;

  return false;
}

// Initializes the buffer to a set size
bool _CircularBuffer_Init(struct CircularBuffer* object, int objectsize, int size)
{
	object->startPos = 0;
	object->endPos = 0;

	if(size <= 0)
		size = 1;

	return _DynamicArray_InitDefault(object->array, objectsize, size+1);
}

// Returns true if the buffer it completely empty
bool CircularBufferIsEmpty(struct CircularBuffer* object)
{
	if(object->startPos == object->endPos)
		return true;

	return false;
}

// Returns true if the buffer is full
bool CircularBufferIsFull(struct CircularBuffer* object)
{
	if( (object->endPos+1)%CircularBufferSize(object) == object->startPos)
		return true;

  return false;
}

// Returns the size of the buffer
int CircularBufferSize(struct CircularBuffer* object)
{
	return DynamicArrayAllocatedSize(&object->array);
}

// Adds a new element to a circular buffer
bool _CircularBuffer_Add(struct CircularBuffer* object, intptr_t* value)
{
	if(CircularBufferIsFull(object))
	{
		BNS_ERROR("CIRCULAR BUFFER", "CIRCULAR BUFFER IS FULL in CircularBufferAdd(...)");
		return false;
	}
	DynamicArraySet(&object->array, object->endPos, value);
	object->endPos = (object->endPos+1)%CircularBufferSize(object);
	return true;
}

// Gets the next element on the circular buffer
intptr_t CircularBufferGet(struct CircularBuffer* object)
{
	if(CircularBufferIsEmpty(object))
	{
		BNS_ERROR("CIRCULAR BUFFER", "CIRCULAR BUFFER IS EMPTY in CircularBufferGet(...)");
		return 0;
	}

	intptr_t returnResult = DynamicArrayGet(&object->array, object->startPos);
	object->startPos = (object->startPos+1)%CircularBufferSize(object);

	return returnResult;
}
#endif
