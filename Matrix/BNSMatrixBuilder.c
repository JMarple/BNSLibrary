float getBufferElement(int element)
{
	return MatBuilder.buffer[element+1];
}

bool setBufferElement(int element, float value)
{
	if(element < sizeof(MatBuilder.buffer) / sizeof(float))
	{
	  MatBuilder.buffer[element+1] = value;
	  return true;
	}

	return false;
}

#define MEM_FREE_BIT 20

void initMemory()
{
	MatBuilder.buffer[0] = sizeof(MatBuilder.buffer) / sizeof(MatBuilder.buffer[0]);
}

bool bnsIsFree(int space)
{
	if(space >= sizeof(MatBuilder.buffer) / sizeof(MatBuilder.buffer[0]))
		return false;

	return !(((int)MatBuilder.buffer[space] >> MEM_FREE_BIT)&0x01);
}

int bnsGetData(int space)
{
	return (int)(MatBuilder.buffer[space]) & ~(1 << MEM_FREE_BIT);
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
			MatBuilder.buffer[memloc] = bnsGetData(memloc) + bnsGetData(memloc+sizeOfSpace);
			MatBuilder.buffer[memloc+sizeOfSpace] = 0;
		}
		else
		{
			// Check next chunk of memory
			memloc += sizeOfSpace;

			// Return null if no suitable space was found!
			if(memloc >= sizeof(MatBuilder.buffer) / sizeof(MatBuilder.buffer[0]))
				return;
		}
	}
}

void bnsFree(int space)
{
	int x = MatBuilder.buffer[space];
	x = x & ~(1 << MEM_FREE_BIT);
	MatBuilder.buffer[space] = x;
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
			MatBuilder.buffer[memloc] = x;
			MatBuilder.buffer[memloc+size+1] = sizeOfSpace - (size+1);

			return memloc;
		}
		else
		{
			// Check next chunk of memory
			memloc += sizeOfSpace;

			// Return null if no suitable space was found!
			if(memloc >= sizeof(MatBuilder.buffer) / sizeof(MatBuilder.buffer[0]))
				return -1;
		}
	}
}

void bnsPrintMemory(int s, int e)
{
	for(int i = s; i < e; i++)
	{
		if((i-s)%4 == 0)
			writeDebugStreamLine("");

		if(bnsIsFree(i))
			writeDebugStream("*");

		writeDebugStream("%d ", bnsGetData(i));

	}
	writeDebugStreamLine("");
}
