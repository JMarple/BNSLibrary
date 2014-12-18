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
// 
// Dependencies:
//    BNSHeap.h.h

#pragma systemFile

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
    // Bug->Negative numbers will contain this bit, ugh
    // Prevent corrupting heap by overwriting headers
    //if(bnsIsProtected(element+1))
      //return false;

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

  // Erase information (could be slow.. might want to remove this)
  //for(int i = space+1; i < space+bnsGetData(space); i++)
    //bnsHeap[i] = 0;

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

    // Find a free piece of memory that is large enough
    if(bnsIsFree(memloc) && sizeOfSpace > size+1)
    {
      int x = size+1;
      x |= (1 << MEM_FREE_BIT);
      x |= (1 << MEM_PROT_BIT);
      bnsHeap[memloc] = x;
      bnsHeap[memloc+size+1] = (sizeOfSpace - (size+1)) | (1 << MEM_PROT_BIT);

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

// Print Heap Memory
void bnsPrintMemory(int startPos, int endPos)
{
  for(int i = startPos; i < endPos; i++)
  {
    if((i-startPos)%4 == 0)
      writeDebugStreamLine("");

    if(bnsIsFree(i) && bnsIsProtected(i))
      writeDebugStream("*");

    if(bnsIsProtected(i))
      writeDebugStream("|");

    writeDebugStream("%d ", bnsGetData(i));

  }
  writeDebugStreamLine("");
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
