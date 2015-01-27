#ifndef __BNS_DATA_STRUCTURES_H
#define __BNS_DATA_STRUCTURES_H

#define DEFAULT_DYNAMIC_ARRAY_SIZE 50

struct DynamicArray
{
	int size;
	int maxSize;
  bool inUse;
	int pointer;
};

void DynamicArrayClear(struct DynamicArray *array);
float DynamicArrayGet(struct DynamicArray *array, int where);
bool DynamicArrayAdd(struct DynamicArray *array, float value);
void DynamicArrayInit(struct DynamicArray *array);
bool DynamicArrayRemoveAt(struct DynamicArray *array, int where);

#endif
