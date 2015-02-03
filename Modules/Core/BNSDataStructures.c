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
bool DynamicArrayInit(struct DynamicArray *array)
{
  DynamicArrayClear(array);

  array->pointer = bnsMalloc(DEFAULT_DYNAMIC_ARRAY_SIZE);

  if(array->pointer != -1)
  {
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
bool DynamicArrayInitDefault(struct DynamicArray *array, int setSize)
{
	DynamicArrayClear(array);

	array->pointer = bnsMalloc(setSize);

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

// Automatically adds a new element into DynamicArray
// If new space is needed, it will automatically allocate that
//   memory
bool DynamicArrayAdd(struct DynamicArray *array, float value)
{
  if(array->size < array->maxSize)
  {
    bnsSetHeapElement(array->pointer + array->size, value);
    array->size++;
  }
  else
  {
    //writeDebugStreamLine("%d Trying to extend", array->size);
    int res = bnsExtend(array->pointer, array->maxSize*2);
    if(res != -1)
    {
      array->pointer = res;
      array->maxSize *= 2;
      bnsSetHeapElement(array->pointer + array->size, value);
      array->size++;
    }
    else
    {
      BNS_ERROR("DYNAMIC ARRAY", "Ran out of memory! Check DynamicArrayAdd(...)");
      return false;
    }
  }

  return true;
}

// Sets a point in memory
void DynamicArraySet(struct DynamicArray *array, int where, float value)
{
	if(where < array->maxSize)
	{
		// This makes sure the pointer to where to add data isn't
	  //  overwriting data
		if(where >= array->size)
			array->size = where+1;

		bnsSetHeapElement(array->pointer + where, value);
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

	return true;
}

// Copy a matrix by allocating new memory for it
bool DynamicArrayCopyByValue(struct DynamicArray *dst, struct DynamicArray src)
{
	int i;
	DynamicArrayDelete(dst);
	DynamicArrayInitDefault(dst, src.maxSize);

	dst->inUse = true;
	for(i = 0; i < dst->maxSize; i++)
	{
		DynamicArraySet(dst, i, DynamicArrayGet(&src, i));
	}

	dst->size = src.size;
	return true;
}

// Retrivies a point of memory from the array
float DynamicArrayGet(struct DynamicArray *array, int where)
{
  return bnsGetHeapElement(array->pointer + where);
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
  return array->size;
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
bool StackInit(struct Stack *object)
{
  object->pos = -1;
  return DynamicArrayInit(object->array);
}

// Removes the top-most number and returns it
//  to the user
float StackPop(struct Stack *object)
{
  if(object->pos < 0)
  {
    BNS_ERROR("STACK ERROR", "Cannot pop a stack of size zero!");
    return 0;
  }

  float returnVal = DynamicArrayGet(object->array, object->pos);
  object->pos--;

  return returnVal;
}

// Puts an element to the top of the stack
bool StackPush(struct Stack *object, float value)
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
float StackPeek(struct Stack *object)
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
bool CircularBufferInit(struct CircularBuffer* object, int size)
{
	object->startPos = 0;
	object->endPos = 0;

	if(size <= 0)
		size = 1;

	return DynamicArrayInitDefault(object->array, size+1);
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
bool CircularBufferAdd(struct CircularBuffer* object, float value)
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
float CircularBufferGet(struct CircularBuffer* object)
{
	if(CircularBufferIsEmpty(object))
	{
		BNS_ERROR("CIRCULAR BUFFER", "CIRCULAR BUFFER IS EMPTY in CircularBufferGet(...)");
		return 0;
	}
	float returnResult = DynamicArrayGet(&object->array, object->startPos);
	object->startPos = (object->startPos+1)%CircularBufferSize(object);
	return returnResult;
}
#endif
