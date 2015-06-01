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

	writeDebugStreamLine("\nDynamic Array Struct Example: Adding elements to array");

	// Set this Dynamic Array to carry "Stack" types.  Note: DynamicArrayInit
	//  is a macro that allows for a "cleaner" way of defining what type a
	//  dynamic array is using.
	DynamicArray arr2;
	DynamicArrayInit(&arr2, Stack);

	// There are two ways of adding data to a stack.  The first is as follows:
	// Define a stack structure the "usual" way and add it to the dynamic array
	Stack lolz;
	StackInit(lolz, float);

	float num1 = 99;
	float num2 = 8.3425243;
	StackPush(lolz, &num1);
	StackPush(lolz, &num2);

	// The second way of adding data to the stack as follows:
	// Add the stack to the dynamic array. Note: the contents at
	//  the pointer &lolz is copied to the heap so you can reuse
	//  the variale "lolz" for a new stack if wanted.
	DynamicArrayAdd(&arr2, &lolz);

	// Define a stack structure "dynamically" without defining a
	//   Stack object at compile time
	DynamicArrayAddEmpty(&arr2);
	//   Note: DynamicArrayGet will return an intprt_t pointer, so
	//   if you want to use it for a Stack, you must typecast it to
	//   (Stack*)
	Stack* x = (Stack*)DynamicArrayGet(&arr2, 1);
	StackInit(x, float); // Use the stack 'x' as you would for a normal stack/object

	float num3 = 123;
	float num4 = 456;
	float num5 = 789;
	StackPush(x, &num3);
	StackPush(x, &num4);
	StackPush(x, &num5);

	// Remote all elements from stack 0
	x = (Stack*)DynamicArrayGet(&arr2, 0);
	while(!StackIsEmpty(x))
	{
		float* y = StackPop(x);
		writeDebugStreamLine("%f", *y);
	}

	// Remove all elements from stack 1
	x = (Stack*)DynamicArrayGet(&arr2, 1);
	while(!StackIsEmpty(x))
	{
		float* y = StackPop(x);
		writeDebugStreamLine("%f", *y);
	}

	// Initilize the stack
	writeDebugStreamLine("\nStack Example: Pushing/Popping 5, 45, and 88 to the stack");
	Stack stack1;
	StackInit(&stack1, float);

	// Add elements to the stack
	float num6 = 5;
	float num7 = 45;
	float num8 = 88;
	StackPush(&stack1, &num6);
	StackPush(&stack1, &num7);
	StackPush(&stack1, &num8);

	// Pop and display the entire contents of the stack
	while(!StackIsEmpty(&stack1))
	{
		float* y = StackPop(&stack1);
		writeDebugStreamLine("%d", *y);
	}

	writeDebugStreamLine("\nCircular Buffer Example: Adding/Reading 3.1, 3.7, 4.1");

	// Create a circular buffer of 3 elements
	CircularBuffer cir1;
	CircularBufferInit(&cir1, float, 3);

	// Add elements to the buffer
	float num9 = 3.1;
	float num10 = 3.7;
	float num11 = 4.1;
	CircularBufferAdd(&cir1, &num9);
	CircularBufferAdd(&cir1, &num10);
	CircularBufferAdd(&cir1, &num11);

	// Read out all the elements in the buffer
	while(!CircularBufferIsEmpty(&cir1))
	{
		float* y = CircularBufferGet(&cir1);
		writeDebugStreamLine("%f", *y);
	}
}
