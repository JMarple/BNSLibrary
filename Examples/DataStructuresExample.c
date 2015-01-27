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
  writeDebugStreamLine("Dynamic Array Example: Adding elements");
  DynamicArray arr1;
  DynamicArrayInit(&arr1);

  for(int i = 0; i < 5; i++)
  {
  	// Add i*5 to the array
  	DynamicArrayAdd(&arr1, i*5);
  }

  // Remove an element cause whynot
  DynamicArrayRemoveAt(&arr1, 2);

  // Display all the element in the array
  for(int i = 0; i < DynamicArraySize(&arr1); i++)
  	writeDebugStreamLine("Element at %d is %d", i, DynamicArrayGet(&arr1, i));


  // Initilize the stack
  writeDebugStreamLine("Stack Example: Pushing/Popping 5, 45, and 88 to the stack");
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
}
