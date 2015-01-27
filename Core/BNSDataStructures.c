#pragma systemFile

#ifndef __BNS_DATA_STRUCTURES_C
#define __BNS_DATA_STRUCTURES_C

#ifndef __BNS_DATA_STRUCTURES_H
#include "BNSDataStructures.h"
#endif

void DynamicArrayInit(struct DynamicArray *array)
{
	DynamicArrayClear(array);

	array->pointer = bnsMalloc(DEFAULT_DYNAMIC_ARRAY_SIZE);

	if(array->pointer != -1)
	{
		array->maxSize = DEFAULT_DYNAMIC_ARRAY_SIZE;
		array->size = 0;
		array->inUse = true;
	}
}

bool DynamicArrayAdd(struct DynamicArray *array, float value)
{
	if(array->size < array->maxSize)
	{
		bnsSetHeapElement(array->pointer + array->size, value);
		array->size++;
	}
	else
	{
		writeDebugStreamLine("%d Trying to extend", array->size);
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
			return false;
		}
	}

	return true;
}

float DynamicArrayGet(struct DynamicArray *array, int where)
{
	return bnsGetHeapElement(array->pointer + where);
}

bool DynamicArrayRemoveAt(struct DynamicArray *array, int where)
{
	if(where < array->maxSize && where >= 0)
  {
  	for(int i = where; i < array->maxSize-1; i++)
  	{
  		bnsSetHeapElement(array->pointer + i, bnsGetHeapElement(array->pointer + i+1));
  	}
  	bnsSetHeapElement(array->pointer + array->maxSize-1, 0);
  }
  else
  {
    writeDebugStreamLine("***\nBNS DYNAMIC ARRAY ERROR\nTrying to remove unallocated memory, check calls to DynamicArrayRemoveAt(...)\n***\n");
  	return false;
  }

  return true;
}

void DynamicArrayClear(struct DynamicArray *array)
{
	array->size = 0;
	if(array->inUse == true)
	{
		bnsFree(array->pointer);
		array->pointer = -1;
	}
}

#endif
