#pragma systemFile

struct MedianFilter
{
	Matrix data;
};

void MedianInitialize(MedianFilter filter, int size)
{
	DeleteMatrix(filter.data);
	CreateZerosMatrix(filter.data, size, 1);
}

void MedianAddData(MedianFilter filter, float data)
{
	for(int i = filter.data.m-1; i > 0; i--)
		SetMatrixAt(filter.data, i, 0, GetMatrixAt(filter.data, i-1, 0));

	SetMatrixAt(filter.data, 0, 0, data);
}

float MedianCompute(MedianFilter filter)
{
	Matrix sorted;
	CopyMatrixByValue(sorted, filter.data);

	// Selection Sort
	for(int i = 0; i < sorted.m; i++)
	{
		int lowestIndex = i;
		for(int j = i+1; j < sorted.m; j++)
		{
			if(GetMatrixAt(sorted, j, 0) < GetMatrixAt(sorted, lowestIndex, 0))
					lowestIndex = j;
		}
		float tmpValue = GetMatrixAt(sorted, i, 0);
		SetMatrixAt(sorted, i, 0, GetMatrixAt(sorted, lowestIndex, 0));
		SetMatrixAt(sorted, lowestIndex, 0, tmpValue);
	}

	// Get Median
	int middle = sorted.m / 2;
	float returnValue = GetMatrixAt(sorted, middle, 0);

	DeleteMatrix(sorted);

	return returnValue;
}
