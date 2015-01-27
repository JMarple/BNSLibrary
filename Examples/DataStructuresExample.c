#include "..\BNSLib.h"

task main()
{
  BNS();

	DynamicArray arr1;
	DynamicArray arr2;
	DynamicArray arr3;

	DynamicArrayInit(&arr1);
	DynamicArrayInit(&arr2);

	for(int i = 0; i < 105; i++)
	{
		DynamicArrayAdd(&arr1, i);
	}

	//DynamicArrayRemoveAt(&arr1, -1);
	DynamicArrayClear(&arr1);
	DynamicArrayInit(&arr3);

	for(int i = 0; i < 25; i++)
	{
		DynamicArrayAdd(&arr3, 1001);
	}

	for(int i = 0; i < 105; i++)
	{
		writeDebugStreamLine("%d", DynamicArrayGet(&arr1, i));
	}


}
