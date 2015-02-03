// BNSHeap.c
//
// Author: Justin Marple with Team BNS
// Contact: jmarple@umass.edu
// Date: 12/18/2014
//
// This source file includes source code that
// implements a memory heap for use in
// Vex Robotics Competition.  These operations
// include:
//  - bnsMalloc
//  - bnsFree
//  - bnsGetHeapElement
//  - bnsSetHeapElement
// This heap is implemented by setting the size
// of each "chunk" as the first element.  The
// data is then every consecutive element in that
// chunk-size. Traverseing the heap requires you
// to start from the initial chunk and increase
// the heap location by the size of that chunk
// to find the size of th next chunk of memory
//
// Dependencies:
//    BNSHeap.h
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

#ifndef __BNS_HEAP_C
#define __BNS_HEAP_C

#ifndef __BNS_HEAP_H
#include "BNSHeap.h"
#endif

// Returns an element in the buffer
float bnsGetHeapElement(int element)
{
  // Since our "0" element marks the size
  //  of the chunk size, we have to increment
  //  by one to get the real data that starts at "1"
  return bnsHeap[element+1];
}

// Sets an element in the heap
bool bnsSetHeapElement(int element, float value)
{
  if(element < sizeof(bnsHeap) / sizeof(bnsHeap[0]))
  {
    bnsHeap[element+1] = value;
    return true;
  }

  return false;
}

// Start the memory with 1 chunk that spans the whole heap
void initMemory()
{
  bnsHeap[0] = (int)(sizeof(bnsHeap) / sizeof(bnsHeap[0]) | (1 << MEM_PROT_BIT));
}

// Checks if the "free" bit is set at a memory location
bool bnsIsFree(int loc)
{
  if(loc >= sizeof(bnsHeap) / sizeof(bnsHeap[0]))
    return false;

  return !(((int)bnsHeap[loc] >> MEM_FREE_BIT)&0x01);
}

// Not really used..
bool bnsIsProtected(int space)
{
  if(space >= sizeof(bnsHeap) / sizeof(bnsHeap[0]))
    return false;

  return (bool)(((int)bnsHeap[space] >> MEM_PROT_BIT)&0x01);
}

// Returns data a memory location with the free
//  and protection bits removed
int bnsGetData(int loc)
{
  int data = (int)(bnsHeap[loc]);
  data &= ~(1 << (MEM_FREE_BIT));
  data &= ~(1 << (MEM_PROT_BIT));
  return data;
}

// Defrag our heap by finding free chunks that can be combined
//   into larger free chunks
void bnsDefrag()
{
  int memloc = 0;
  while(true)
  {
    int sizeOfSpace = bnsGetData(memloc);

    // Check if we have two free spaces in a row
    if(bnsIsFree(memloc) && bnsIsFree(memloc+sizeOfSpace))
    {
      bnsHeap[memloc] = bnsGetData(memloc) + bnsGetData(memloc+sizeOfSpace);
      bnsHeap[memloc] = (int)(bnsHeap[memloc]) | (1 << MEM_PROT_BIT);
      bnsHeap[memloc+sizeOfSpace] = 0;
    }
    else
    {
      // Check next chunk of memory
      memloc += sizeOfSpace;

      // Return null if no suitable space was found!
      if(memloc >= sizeof(bnsHeap) / sizeof(bnsHeap[0]))
        return;
    }
  }
}

// Free memory at a certain location
void bnsFree(int loc)
{
  if(loc < 0 || loc > sizeof(bnsHeap)/sizeof(bnsHeap[0]))
    return;

  // Add free bit at the memory location
  int x = bnsHeap[loc];
  x = x & ~(1 << MEM_FREE_BIT);
  bnsHeap[loc] = x;

  // Could be slow, but guarentees we have largest possible chunk sizes
  bnsDefrag();
}

// Try to allocate space in the BNS Heap
int bnsMalloc(int size)
{
  int memloc = 0;

  while(true)
  {
    int sizeOfSpace = bnsGetData(memloc);

    // Size of space should never be <= 0
    if(sizeOfSpace <= 0)
    {
    	while(1==1){delay(1);}
    }

    // Find a free piece of memory that is large enough
    // sizeOfSpace will be the size of the space + 1, so
    //  if it larger then the requested size, it'll be
    //  large enough
    if(bnsIsFree(memloc) && sizeOfSpace > size+1)
    {
      int x = size+1;
      x |= (1 << MEM_FREE_BIT);
      x |= (1 << MEM_PROT_BIT);
      bnsHeap[memloc] = x;

      // If this is equal to zero, we have the exact chunk size we need, but we
      //   don't want to overwrite the next section. Only point to the next section
      //   if there is space to do so
      if(sizeOfSpace - (size+1) > 0)
      {
        bnsHeap[memloc+size+1] = (sizeOfSpace - (size+1)) | (1 << MEM_PROT_BIT);
		  }

      return memloc;
    }
    else
    {
      // Check next chunk of memory
      memloc += sizeOfSpace;

      // Return null if no suitable space was found!
      if(memloc >= sizeof(bnsHeap) / sizeof(bnsHeap[0]))
      {
        writeDebugStreamLine("***\nBNS HEAP ERROR\nNot enough memory for chunk size %d\n***\n", size);
        return -1;
      }
    }
  }
}

// Extends a previous chunk of memory if possible
int bnsExtend(int loc, int totalSize)
{
  int sizeOfSpace = bnsGetData(loc);

  // Check if we need to actually extend
  if(sizeOfSpace >= totalSize)
    return loc;

  int sizeOfNextSpace = bnsGetData(loc+sizeOfSpace);

  // Check if there is enough free memory directly after original memory
  if(sizeOfSpace + sizeOfNextSpace >= totalSize && bnsIsFree(loc+sizeOfSpace) == true)
  {
    int x = totalSize + 1;
    x |= (1 << MEM_FREE_BIT);
    x |= (1 << MEM_PROT_BIT);
    bnsHeap[loc] = x;
    bnsHeap[loc+totalSize+1] = (sizeOfNextSpace - (totalSize - sizeOfSpace + 1));// | (1 << MEM_PROT_BIT);;
    return loc;
  }
  else
  {
    // We have to find a new section of memory if we reach this point
    int memloc = bnsMalloc(totalSize);

    // Check that we actually got a new memory space
    if(memloc == -1)
      return -1;

    // Figure out how much to copy from the old memory location
    int sizeToCopy = bnsGetData(loc);

    // Copy all elements from previous
    for(int i = 0; i < sizeToCopy; i++)
    {
      bnsSetHeapElement(memloc+i, bnsGetHeapElement(loc+i));
    }

    // Free the previous memory
    bnsFree(loc);

    return memloc;
  }
}

// Print Heap Memory
void bnsPrintMemory(int startPos, int endPos)
{
  int i;
  for(i = startPos; i < endPos; i++)
  {
    if((i-startPos)%4 == 0)
      writeDebugStreamLine("");

    if(bnsIsFree(i) && bnsIsProtected(i))
      writeDebugStream("*");

    if(bnsIsProtected(i))
      writeDebugStream("|");

    writeDebugStream("%d ", bnsGetData(i));

  }
  writeDebugStreamLine("\n");
}

// Print Heap Report (good for finding memory leaks)
void bnsPrintMemoryDetails()
{
  writeDebugStreamLine("BNS Memory Allocation: ");
  int heapSize = sizeof(bnsHeap) / sizeof(bnsHeap[0]);
  writeDebugStreamLine(" -- Total Heap Size: \t%d", heapSize);

  int memloc = 0;
  int objCount = 0;
  int free = 0;
  int used = 0;

  while(true)
  {
    int sizeOfSpace = bnsGetData(memloc);

    if(bnsIsFree(memloc))
      free += sizeOfSpace;
    else
      used += sizeOfSpace;

    memloc += sizeOfSpace;
    if(memloc >= heapSize)
      break;
    objCount++;
  }

  writeDebugStreamLine(" -- Number of Chunks: %d", objCount);
  writeDebugStreamLine(" -- Free Memory: \t\t\t%d%% (%d / %d)", (float)free*100.0/(float)(heapSize), free, heapSize);
  writeDebugStreamLine("");
}

#endif
