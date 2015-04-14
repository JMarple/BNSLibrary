// DataStructuresExample.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 01/27/2015
//
// This example file shows how to use
// the Data structures built into BNSLib
//
// Dependencies:
//   BNSLib.h in previous directory
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
#include "..\BNSLib.h"

task main()
{
	// Initialize the BNS Library
  BNS();

  // Initialize the dynamic array
  /*writeDebugStreamLine("\nDynamic Array Example: Adding elements to array");
  DynamicArray arr1;
  DynamicArrayInit(&arr1, float);

  for(int i = 0; i < 5; i++)
  {
  	// Add i*5 to the array
  	DynamicArraySet(&arr1, i, i*5);
  }

  // Remove an element cause whynot
  DynamicArrayRemoveAt(&arr1, 2);

  // Display all the element in the array
  for(int i = 0; i < DynamicArraySize(&arr1); i++)
  {
  	float* x = DynamicArrayGet(&arr1, i);
  	writeDebugStreamLine("Element at %d is %d", i, *x);
	}*/

	writeDebugStreamLine("\nDynamic Array Struct Example: Adding elements to array");

	// Set this Dynamic Array to carry "Stack" types.  Note: DynamicArrayInit
	//  is a macro that allows for a "cleaner" way of defining what type a
	//  dynamic array is using.
	DynamicArray arr2;
	DynamicArrayInit(&arr2, Stack);

	// There are two ways of adding data to a stack.  The first is as follows:
	// Define a stack structure the "usual" way and add it to the dynamic array
	Stack lolz;
	StackInit(lolz);
	StackPush(lolz, 9.9);
	StackPush(lolz, 8.3425243);

	// The second way of adding data to the stack as follows:
	// Add the stack to the dynamic array. Note: the contents at
	//  the pointer &lolz is copied to the heap so you can reuse
	//  the variale "lolz" for a new stack if wanted.
	DynamicArrayAdd(&arr2, (intptr_t*)&lolz);

	// Define a stack structure "dynamically" without defining a
	//   Stack object at compile time
	DynamicArrayAddEmpty(&arr2);
	//   Note: DynamicArrayGet will return an intprt_t pointer, so
	//   if you want to use it for a Stack, you must typecast it to
	//   (Stack*)
	Stack* x = (Stack*)DynamicArrayGet(&arr2, 1);
	StackInit(x); // Use the stack 'x' as you would for a normal stack/object
	StackPush(x, 123);
	StackPush(x, 456);
	StackPush(x, 789);

	// Remote all elements from stack 0
	x = (Stack*)DynamicArrayGet(&arr2, 0);
	while(!StackIsEmpty(x))
		writeDebugStreamLine("%f", StackPop(x));

	// Remove all elements from stack 1
	x = (Stack*)DynamicArrayGet(&arr2, 1);
	while(!StackIsEmpty(x))
			writeDebugStreamLine("%f", StackPop(x));

  // Initilize the stack
  writeDebugStreamLine("\nStack Example: Pushing/Popping 5, 45, and 88 to the stack");
  Stack stack1;
  StackInit(&stack1);

  // Add elements to the stack
  StackPush(&stack1, 5);
  StackPush(&stack1, 45);
  StackPush(&stack1, 88);

  // Pop and display the entire contents of the stack
  while(!StackIsEmpty(&stack1))
  {
  	  writeDebugStreamLine("%d", StackPop(&stack1));
  }

  writeDebugStreamLine("\nCircular Buffer Example: Adding/Reading 3.1, 3.7, 4.1");

  // Create a circular buffer of 3 elements
  CircularBuffer cir1;
  CircularBufferInit(&cir1, 3);

  // Add elements to the buffer
  CircularBufferAdd(&cir1, 3.1);
  CircularBufferAdd(&cir1, 3.7);
  CircularBufferAdd(&cir1, 4.1);

  // Read out all the elements in the buffer
  while(!CircularBufferIsEmpty(&cir1))
		writeDebugStreamLine("%f", CircularBufferGet(&cir1));
}
