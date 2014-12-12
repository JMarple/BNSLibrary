int pushBufferLocation(int spaces)
{
	while(MatBuilder.locked){;}

	if (MatBuilder.head + spaces > sizeof(MatBuilder.buffer)/sizeof(float) )
	{
		return -1; // Not enough space!!
	}

  int tmp = MatBuilder.head;
  MatBuilder.head += spaces;
  return tmp;
}

void popBufferLocation(int spaces)
{
	MatBuilder.head -= spaces;
}

// Prevent threads from overwriting data on the buffer
void lockBuffer()
{
	while(MatBuilder.locked){;}
	MatBuilder.locked = true;
}

void unlockBuffer()
{
	MatBuilder.locked = false;
}

float getBufferElement(int element)
{
	return MatBuilder.buffer[element];
}

bool setBufferElement(int element, float value)
{
	if(element < sizeof(MatBuilder.buffer) / sizeof(float))
	{
	  MatBuilder.buffer[element] = value;
	  return true;
	}

	return false;
}
