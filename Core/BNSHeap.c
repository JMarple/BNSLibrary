#pragma systemFile

bool bnsIsProtected(int space);

float getBufferElement(int element)
{
	return bnsHeap[element+1];
}

bool setBufferElement(int element, float value)
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

void initMemory()
{
	bnsHeap[0] = (int)(sizeof(bnsHeap) / sizeof(bnsHeap[0]) | (1 << MEM_PROT_BIT));
}

bool bnsIsFree(int space)
{
	if(space >= sizeof(bnsHeap) / sizeof(bnsHeap[0]))
		return false;

	return !(((int)bnsHeap[space] >> MEM_FREE_BIT)&0x01);
}

bool bnsIsProtected(int space)
{
	if(space >= sizeof(bnsHeap) / sizeof(bnsHeap[0]))
		return false;

	return (bool)(((int)bnsHeap[space] >> MEM_PROT_BIT)&0x01);
}

int bnsGetData(int space)
{
	int data = (int)(bnsHeap[space]);
	data &= ~(1 << (MEM_FREE_BIT));
	data &= ~(1 << (MEM_PROT_BIT));
	return data;
}

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

void bnsFree(int space)
{
	if(space < 0 || space > sizeof(bnsHeap)/sizeof(bnsHeap[0]))
		return;

	int x = bnsHeap[space];
	x = x & ~(1 << MEM_FREE_BIT);
	bnsHeap[space] = x;

	// Erase information (could be slow.. might want to remove this)
	//for(int i = space+1; i < space+bnsGetData(space); i++)
		//bnsHeap[i] = 0;

	bnsDefrag();
}

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
				writeDebugStreamLine("***\nBNS MATRIX ERROR\nNot enough memory for chunk size %d\n***\n", size);
				return -1;
			}
		}
	}
}

void bnsPrintMemory(int s, int e)
{
	for(int i = s; i < e; i++)
	{
		if((i-s)%4 == 0)
			writeDebugStreamLine("");

		if(bnsIsFree(i) && bnsIsProtected(i))
			writeDebugStream("*");

		if(bnsIsProtected(i))
			writeDebugStream("|");

		writeDebugStream("%d ", bnsGetData(i));

	}
	writeDebugStreamLine("");
}

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
